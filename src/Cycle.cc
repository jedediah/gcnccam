/*
	File: Cycle.cc
	Copyright (C) 2008 Arnd Begemann

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "Cycle.h"
#include "Line.h"
#include "Arc.h"
#include <assert.h>
#include <cmath>
#include <math.h>
Cycle::Cycle() 
{
	m_property=NULL;
	dir_ccw=false;
}


Cycle::Cycle(const Graph& g) 
	//: Graph(g), dir_ccw(false)
{
	cItVertexList itV;
	for(itV=g.vertices.begin(); itV!=g.vertices.end(); itV++)
	{
		Vertex* temp_vertex = *itV;
		if(temp_vertex!=NULL)
		{
			vertices.push_back(new Vertex(*temp_vertex));
		}
	}	
	cItShapeList itS;
	for(itS=g.shapes.begin(); itS!=g.shapes.end(); itS++)
	{
		Shape* temp_shape= *itS;
		if(temp_shape!=NULL)
		{
			shapes.push_back(temp_shape->clone());
		}
	}
	dir_ccw=false;
	m_property=g.getProperty();
}

Cycle::Cycle(const Cycle& g) 
	//: Graph(c), dir_ccw(c.dir_ccw)
{
	cItVertexList itV;
	for(itV=g.vertices.begin(); itV!=g.vertices.end(); itV++)
	{
		Vertex* temp_vertex = *itV;
		if(temp_vertex!=NULL)
		{
			vertices.push_back(new Vertex(*temp_vertex));
		}
	}	
	cItShapeList itS;
	for(itS=g.shapes.begin(); itS!=g.shapes.end(); itS++)
	{
		Shape* temp_shape= *itS;
		if(temp_shape!=NULL)
		{
			shapes.push_back(temp_shape->clone());
		}
	}
	dir_ccw=g.dir_ccw;
	m_property=g.getProperty();
}

Cycle::~Cycle()
{
	// TODO: put destructor code here
}

Graph* Cycle::clone() const
{
	return new Cycle(*this);
}

void Cycle::sortCounterClockWise()
{
//// arange cirles and arc-line
	if(vertices.size()==shapes.size())
	//if(vertices.size() >= 3 && vertices.size()==shapes.size())
	{
		Vertex* v_start=vertices.front();
		assert(v_start);
		lShapeList sl_clone;
		if(sortShapesAsRing(*v_start,sl_clone)) 
		{
			shapes=sl_clone;
			////std::cout << "Shapes resorted" << std::endl;
		}
		sortVerticesLikeShapes();
		if (isCounterClockWise())
			dir_ccw=true;
		else
		{
			dir_ccw=false;
			////std::cout << "Dir not ccw" << std::endl;
			swapDirection();
		}
	}
	else
	{
		std::cout << "Number of: Vetices!=shapes: error!!!";
	}
}

void Cycle::sortClockWise()
{
	sortCounterClockWise();
	swapDirection();
}

void Cycle::swapDirection()
{
	
	shapes.reverse();
//	vertices.reverse();
	for(cItShapeList it_s=shapes.begin(); it_s!=shapes.end(); it_s++)
	{
		Shape* s_temp=*it_s;
		assert(s_temp);
		s_temp->swapDirection();	
	}
	sortVerticesLikeShapes();
	if(dir_ccw==true)
		dir_ccw=false;
	else
		dir_ccw=true;
}


void Cycle::sortVerticesLikeShapes()
{
	for(cItShapeList it_s=shapes.begin(); it_s!=shapes.end(); it_s++)
	{
		Shape* s_temp=*it_s;
		assert(s_temp);
		Vertex* v_temp=findVertex(s_temp->getStartPoint());
		if(v_temp!=NULL)
		{
			vertices.remove(v_temp);
			vertices.push_back(v_temp);
		}
	}	
}

std::ostream& Cycle::getCncCode(std::ostream &os, unsigned int& jump_mark)
{
	if( isCircle() && 
		((m_property->getTool()->getDiameter()==2.0*((Arc*)shapes.front())->getRadius())
			|| (  m_property->getForceDrillSmaller() 
					&& m_property->getTool()->getDiameter()>=2.0*((Arc*)shapes.front())->getRadius())))
	{
		// move to center of the circle
		os << "G0" << " X" << ((Arc*)shapes.front())->getCenterPoint().x << " Y" << ((Arc*)shapes.front())->getCenterPoint().y << std::endl;
		// drill
		// first fast Move to 0.0
		os << "G0" << " Z" << 0.0 << std::endl;
		// slow move to Z-Position with Z FEED RATE
		if(m_property->getZFeedRate() != 0.0)
			os << "F" << m_property->getZFeedRate() << std::endl; //Z Feed Rate
			double z_end_pos=m_property->getZStartPosition()-m_property->getCutDepth();
			os << "G1" << " Z" << z_end_pos << std::endl; // drill to Z end Position
			// Restore Cut FEED RATE
			if(m_property->getCutFeedRate() != 0.0)
				os << "F" << m_property->getCutFeedRate() << std::endl; //Cutter Feed Rate
	}		
	else // mill 
	{
		int count=(int)(fabs(m_property->getCutDepth())/fabs(m_property->getZPitch()));
		double last_z_pitch = fmod(fabs(m_property->getCutDepth()), fabs(m_property->getZPitch()));
		double signed_z_pitch;
		if(m_property->getCutDepth()<0)
			signed_z_pitch=fabs(m_property->getZPitch());
		else
			signed_z_pitch=-fabs(m_property->getZPitch());
		if(last_z_pitch>0.0)
			++count;
		
		if(shapes.size() > 0)
		{
			Shape* s_back=shapes.back();
			assert(s_back);
			double entry_z_pos;
			if(count>1)
				entry_z_pos=m_property->getZStartPosition()+signed_z_pitch;
			else
				entry_z_pos=m_property->getZStartPosition()-m_property->getCutDepth();
			s_back->getEndCncEntryMove(os, m_property->getZStartPosition(), entry_z_pos);
		}
		double actual_z_pos=m_property->getZStartPosition();
		for(int i=0; i<count;++i)
		{
			if(i==(count-1))
				actual_z_pos=m_property->getZStartPosition()-m_property->getCutDepth(); //last position
			else
				actual_z_pos+=signed_z_pitch;
			if(i>0)
			{
				// slow move to Z-Position with Z FEED RATE
				if(m_property->getZFeedRate() != 0.0)
					os << "F" << m_property->getZFeedRate() << std::endl; //Z Feed Rate
				os << "G1" << " Z" << actual_z_pos << std::endl; // next Z Position of the canned cycle
				// Restore Cut FEED RATE
				if(m_property->getCutFeedRate() != 0.0)
					os << "F" << m_property->getCutFeedRate() << std::endl; //Cutter Feed Rate	
			}
			for(cItShapeList it=shapes.begin(); it!=shapes.end(); ++it)
			{
				Shape* s_temp=*it;
				assert(s_temp);
				s_temp->getCncCode(os, jump_mark);
			}
		}
		os << "G0" << " Z" << 0.0 << std::endl; // first move up with radius compensation on
		os << "G40" << std::endl; // radius compensation off
	}
	return os;
}

bool Cycle::isCircle()
{
	bool ret = false;
	if(shapes.size()==1)
	{
		ret = shapes.front()->isCircle();
	}
	return ret;
}

bool Cycle::sortShapesAsRing(const Vertex& v_start, /*out*/ lShapeList& sl_ring)
{
	bool ret=false;
	if(shapes.size()<2)
		return true;
	lShapeList sl_conn;
	getAllConncectedShapes(sl_conn,*findVertex(v_start.point));
	assert(sl_conn.size()==2);
	Shape *s_temp=sl_conn.front();
	if(s_temp!=NULL)
	{
		if(s_temp->getStartPoint()!=v_start.point)
		s_temp->swapDirection();
		sl_ring.push_back(s_temp);
		for(unsigned int i = 1; i<shapes.size();i++)
		{
			getAllConncectedShapes(sl_conn,*findVertex(s_temp->getEndPoint()));
			assert(sl_conn.size()==2);
			if(sl_conn.size()!=2)
				break;
			for(cItShapeList it_s=sl_conn.begin(); it_s!=sl_conn.end();++it_s)
			{
				Shape* s_conn=*it_s;
				assert(s_conn);
				Shape* s_swap_conn=s_conn->clone();
				s_swap_conn->swapDirection();
				if(*s_conn!=*s_temp && *s_temp!=*s_swap_conn)
				{
					if(s_temp->getEndPoint()==s_conn->getStartPoint())
					{
						s_temp=s_conn;
						sl_ring.push_back(s_temp);
						break;
					}
					if(s_temp->getEndPoint()==s_conn->getEndPoint())
					{
						s_conn->swapDirection();
						s_temp=s_conn;
						sl_ring.push_back(s_temp);
						break;
					}
				}
				if(s_swap_conn)
				{
					delete s_swap_conn;
					s_swap_conn=NULL;
				}
			}
		}
	}
	ret= sl_ring.size()==shapes.size() 
		&& sl_ring.front()->getStartPoint()==sl_ring.back()->getEndPoint();
	return ret;
}

bool Cycle::isCounterClockWise()
{
	bool ret = false;
	if(shapes.size()==1 && typeid(*shapes.front())==typeid(Arc))
	{
		Arc* circle = (Arc*)shapes.front();
		ret = circle->isCounterClockWise();
		return ret;
	}
	if(shapes.size()==2)
	{
		if(typeid(*shapes.front())==typeid(Arc))
		{
			Arc* circle = (Arc*)shapes.front();
			ret = circle->isCounterClockWise();
		}
		else if(typeid(*shapes.back())==typeid(Arc))
		{
			Arc* circle = (Arc*)shapes.back();
			ret = circle->isCounterClockWise();
		}
		return ret;
	}
	Point2D gravity_point = getGravityPoint();
	double a_before=0;
	double a_aktual=0;
	double a_ges=0;
	for(cItShapeList it_s=shapes.begin(); it_s!=shapes.end(); it_s++)
	{
		Shape* s_temp = *it_s;
		assert(s_temp);
		Point2D end_point=s_temp->getEndPoint();
		Line l_temp(gravity_point,end_point);
		a_aktual=l_temp.getAngle();	
		std::cout << "a_aktual:" << a_aktual << std::endl;
		if(it_s!=shapes.begin())
		{
			// since (a_aktual-a_before) have to be < 180 Deg and > -180
			// shift start angle to 0.0
			double a_delta = (a_aktual-a_before);
			if(a_delta > 360.0)
				a_delta-=360.0;
			if(a_delta < -360.0)
				a_delta+=360.0;
			if(a_delta > 180.0)
				a_delta=-360.0+a_delta;
			if(a_delta <= -180.0)
				a_delta=360.0+a_delta;
			assert( a_delta <= 180.0 && a_delta > -180.0);
			std::cout << "a_delta:" << a_delta << std::endl;
			a_ges+=a_delta;
			std::cout << "a_ges:" << a_ges << std::endl;
		}
		a_before=a_aktual;			
	}
	if(a_ges>0.0)
	{
		ret=true;
	}
	return ret;
}
