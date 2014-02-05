#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <assert.h>
#include <utils/point.h>
#include "accessstate.h"
#include "../protobuf/particle_reading.pb.h"

#include <iostream>

#define GSX array2d->m_xsize()
#define GSY array2d->m_ysize()

#define SSX(X) array2d->set_m_xsize(X)
#define SSY(Y) array2d->set_m_ysize(Y)


namespace GMapping {


struct PointAccumulator;


class Array2D_PA{
public:
	Array2D_PA(int xsize=0, int ysize=0);
	Array2D_PA& operator=(const Array2D_PA &);
	Array2D_PA(const Array2D_PA &);
	~Array2D_PA();
	void clear();
	void resize(int xmin, int ymin, int xmax, int ymax);


	inline bool isInside(int x, int y) const;
	inline const PointAccumulator& cell(int x, int y) const;
	inline PointAccumulator& cell(int x, int y);
	inline AccessibilityState cellState(int x, int y) const { return (AccessibilityState) (isInside(x,y)?(Inside|Allocated):Outside);}

	inline bool isInside(const IntPoint& p) const { return isInside(p.x, p.y);}
	inline const PointAccumulator& cell(const IntPoint& p) const {return cell(p.x,p.y);}
	inline PointAccumulator& cell(const IntPoint& p) {return cell(p.x,p.y);}
	inline AccessibilityState cellState(const IntPoint& p) const { return cellState(p.x, p.y);}

	inline int getPatchSize() const{return 0;}
	inline int getPatchMagnitude() const{return 0;}
	inline int getXSize() const {return GSX;}
	inline int getYSize() const {return GSY;}

	gmapping_structs::Array2D * array2d;

	PointAccumulator ** m_cells;
protected:
	//int m_xsize, m_ysize;
private:
	void initCells(bool zero);
};

void Array2D_PA::initCells(bool zero) {
	m_cells = new PointAccumulator*[GSX];
	for (int i = 0; i < GSX; i++) {
		gmapping_structs::Array2D::innerType * x = array2d->add_x();
		m_cells[i] = new PointAccumulator[GSY];
		for (int j = 0; j < GSY; j++) {
			gmapping_structs::PointAccumulator * acc_y = x->add_y();
			if (zero){
				acc_y->set_n(0);
				acc_y->set_visits(0);
				acc_y->set_x(0);
				acc_y->set_y(0);
			}
			m_cells[i][j].pointacc = acc_y;
		}
	}
}

Array2D_PA::Array2D_PA(int xsize, int ysize){
	//	assert(xsize>0);
	//	assert(ysize>0);
	array2d = new gmapping_structs::Array2D();
	SSX(xsize);
	SSY(ysize);
	if (xsize>0 && ysize>0){
		initCells(true);
	}
	else{
		SSX(0);
		SSY(0);
		m_cells=0;
	}
}

Array2D_PA& Array2D_PA::operator=(const Array2D_PA& g){
	if (GSX!=g.GSX || GSY!=g.GSY){

		for (int i=0; i<GSX; i++)
			delete [] m_cells[i];
		delete [] m_cells;

		delete array2d;

		array2d = new gmapping_structs::Array2D();

		SSX(g.GSX);
		SSY(g.GSY);

		initCells(false);
	}

	for (int x=0; x<GSX; x++)
		for (int y=0; y<GSY; y++){
			m_cells[x][y].pointacc->set_x(g.m_cells[x][y].pointacc->x());
			m_cells[x][y].pointacc->set_y(g.m_cells[x][y].pointacc->y());
			m_cells[x][y].pointacc->set_n(g.m_cells[x][y].pointacc->n());
			m_cells[x][y].pointacc->set_visits(g.m_cells[x][y].pointacc->visits());
		}
	return *this;
}

Array2D_PA::Array2D_PA(const Array2D_PA& g){
	SSX(g.GSX);
	SSY(g.GSY);

	array2d = new gmapping_structs::Array2D();

	initCells(false);

	for (int x=0; x<GSX; x++){
		for (int y=0; y<GSY; y++){
			m_cells[x][y].pointacc->set_x(g.m_cells[x][y].pointacc->x());
			m_cells[x][y].pointacc->set_y(g.m_cells[x][y].pointacc->y());
			m_cells[x][y].pointacc->set_n(g.m_cells[x][y].pointacc->n());
			m_cells[x][y].pointacc->set_visits(g.m_cells[x][y].pointacc->visits());
		}
	}
}

Array2D_PA::~Array2D_PA(){

	for (int i=0; i<GSX; i++){
		delete [] m_cells[i];
		m_cells[i]=0;
	}
	delete [] m_cells;
	m_cells=0;
	delete array2d;
	array2d=0;
}

void Array2D_PA::clear(){

	for (int i=0; i<GSX; i++){
		delete [] m_cells[i];
		m_cells[i]=0;
	}
	delete [] m_cells;
	m_cells=0;
	array2d->Clear();
	SSX(0);
	SSY(0);
}


void Array2D_PA::resize(int xmin, int ymin, int xmax, int ymax){
	int xsize=xmax-xmin;
	int ysize=ymax-ymin;

	PointAccumulator ** newcells = new PointAccumulator*[xsize];
	gmapping_structs::Array2D * newarray2d = new gmapping_structs::Array2D();

	for (int i = 0; i < xsize; i++) {
		gmapping_structs::Array2D::innerType * x = newarray2d->add_x();
		newcells[i] = new PointAccumulator[ysize];
		for (int j = 0; j < ysize; j++) {
			gmapping_structs::PointAccumulator * acc_y = x->add_y();
			acc_y->set_n(0);
			acc_y->set_visits(0);
			acc_y->set_x(0);
			acc_y->set_y(0);

			newcells[i][j].pointacc = acc_y;
		}
	}


	int dx= xmin < 0 ? 0 : xmin;
	int dy= ymin < 0 ? 0 : ymin;
	int Dx=xmax<this->GSX?xmax:this->GSX;
	int Dy=ymax<this->GSY?ymax:this->GSY;


	for (int x=dx; x<Dx; x++){
		for (int y=dy; y<Dy; y++){

			//newcells[x-xmin][y-ymin]=this->m_cells[x][y];
			newcells[x-xmin][y-ymin].pointacc->set_n(m_cells[x][y].pointacc->n());
			newcells[x-xmin][y-ymin].pointacc->set_x(m_cells[x][y].pointacc->x());
			newcells[x-xmin][y-ymin].pointacc->set_y(m_cells[x][y].pointacc->y());
			newcells[x-xmin][y-ymin].pointacc->set_visits(m_cells[x][y].pointacc->visits());


		}
		delete [] this->m_cells[x];
	}
	delete [] this->m_cells;
	delete array2d;


	this->m_cells=newcells;
	this->array2d=newarray2d;
	SSX(xsize);
	SSY(ysize);
}


inline bool Array2D_PA::isInside(int x, int y) const{
	return x>=0 && y>=0 && x<GSX && y<GSY;
}

inline const PointAccumulator& Array2D_PA::cell(int x, int y) const{
	assert(isInside(x,y));
	return m_cells[x][y];
}


inline PointAccumulator& Array2D_PA::cell(int x, int y){
	assert(isInside(x,y));
	return m_cells[x][y];
}

};

#endif

