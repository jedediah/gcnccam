/*
	File: Graph.cc
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

#include <math.h>

#include "Graph.h"
#include <assert.h>
#include "Connection.h"
Graph::Graph()
{
	m_property=NULL;
}

Graph::Graph(const Graph& g)
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
	m_property=g.getProperty();
}
Graph::~Graph()
{
	clear();
}

Graph::Graph(const lShapeList& s)
{
	cItShapeList itS;
	for(itS=s.begin(); itS!=s.end(); itS++)
	{
		Shape* temp_shape= *itS;
		if(temp_shape!=NULL)
		{
			shapes.push_back(temp_shape->clone());
		}
	}
	createAllVertices();
}

Graph* Graph::clone() const
{
	return new Graph(*this);
}

void Graph::clear()
{
	clearVertices();
	clearShapes();
}

void Graph::clearVertices()
{
	cItVertexList it;
	for(it=vertices.begin(); it!=vertices.end(); it++)
	{
		Vertex* temp = *it;
		delete temp;
	}
	vertices.clear();
}

void Graph::clearShapes()
{
	cItShapeList it;
	for(it=shapes.begin(); it!=shapes.end(); it++)
	{
		Shape* temp = *it;
		delete temp;
	}
	shapes.clear();
}

std::string Graph::print()
{
	std::stringstream stream;
	stream << std::endl << "Graph" << std::endl;
	stream << "Vertices" << std::endl;
	cItVertexList itV;
	for(itV=vertices.begin(); itV!=vertices.end(); itV++)
	{
		Vertex* v = *itV;
		stream << v->point.print() << std::endl;
	}
	stream << "Shapes" << std::endl;
	cItShapeList itS;
	for(itS=shapes.begin(); itS!=shapes.end(); itS++)
	{
		Shape* s = *itS;
		stream << s->print() << std::endl;
	}
	return stream.str(); 
}

std::string Graph::printExt()
{
	std::stringstream stream;
	stream << std::endl << "Graph" << std::endl;
	stream << "Vertices" << std::endl;
	cItVertexList itV;
	for(itV=vertices.begin(); itV!=vertices.end(); itV++)
	{
		Vertex* v = *itV;
		stream << v->point.print();
		stream << ", dist:" << v->dist;
		Vertex* prev = v->previous;
		if(prev!=NULL)
			stream << ", previous:" << prev->point.print();
		stream<< std::endl;
	}
	stream << "Shapes" << std::endl;
	cItShapeList itS;
	for(itS=shapes.begin(); itS!=shapes.end(); itS++)
	{
		Shape* s = *itS;
		stream << s->print() << std::endl;
	}
	return stream.str(); 
}



bool Graph::addShapeIfNotIn(const Shape& s, bool front)
{
	bool ret=false;
	cItShapeList it;
	for(it=shapes.begin(); it!=shapes.end(); it++)
	{
		Shape* temp = *it;
		assert(temp);
		if(temp!=NULL && *temp==s)
		{
			break;
		}
	}
	if(it==shapes.end()) // not found
	{
		ret=true;
		if(front)
			shapes.push_front(s.clone());
		else
			shapes.push_back(s.clone());
	}

	return ret;
}

bool Graph::addVertexIfNotIn(const Vertex& v, bool front)
{
	bool ret=false;
	cItVertexList it;
	
	for(it=vertices.begin(); it!=vertices.end(); it++)
	{
		Vertex* temp = *it;
		if(temp!=NULL && temp->point==v.point)
		{
			break;
		}
	}
	if(it==vertices.end()) // not found
	{
		ret=true;
		if(front)
			vertices.push_front(new Vertex(v));
		else
			vertices.push_back(new Vertex(v));
	}
	return ret;
}

Vertex* Graph::findVertex(const Point2D p) const
{
	cItVertexList it;
	Vertex* v=NULL;	
	for(it=vertices.begin(); it!=vertices.end(); it++)
	{
		Vertex* temp_v = *it;
		//if( temp_v==NULL)
		//	std::cout << "findVertex: NULL" << std::endl;
		if( temp_v!=NULL && p == temp_v->point )
		{
			//std::cout << "findVertex:" << temp_v->point.print() << std::endl;
			v=temp_v;
			break;
		}
	}
	return v;
}

bool Graph::findVertex(const Vertex& v) const
{
	cItVertexList it;
	bool ret = false;
	for(it=vertices.begin(); it!=vertices.end(); it++)
	{
		Vertex* v_temp=*it;
		if( v_temp!=NULL && v_temp->point==v.point )
		{
			ret=true;
			break;
		}
	}
	return ret;
}
Shape* Graph::findShape(const Shape& s) const
{
	cItShapeList it;
	Shape* s_ret = NULL;
	for(it=shapes.begin(); it!=shapes.end(); it++)
	{
		Shape* s_temp=*it;
		if( s_temp!=NULL && *s_temp==s )
		{
			s_ret=s_temp;
			break;
		}	
	}
	return s_ret;	
}
void Graph::getAllConncectedShapes(lShapeList& shape_list, const Vertex& v)
{
	shape_list.clear();
	cItShapeList it;
	for(it=shapes.begin(); it!=shapes.end(); it++)
	{
		Shape* temp_shape = *it;
		if(temp_shape!=NULL)
		{
			//std::cout << temp_shape->print() << std::endl;
			if(temp_shape->getStartPoint()==v.point)
			{
				shape_list.push_back(temp_shape);
			}
			else if(temp_shape->getEndPoint()==v.point)
			{
				shape_list.push_back(temp_shape);
			}
		}
	}
}

void Graph::getAllConncectedVertices(lVertexList& vertex_list, const Vertex& v)
{
	vertex_list.clear();
	cItShapeList it;
	for(it=shapes.begin(); it!=shapes.end(); it++)
	{
		Shape* temp_shape = *it;
		if(temp_shape!=NULL)
		{
			//std::cout << temp_shape->print() << std::endl;
			if(temp_shape->getStartPoint()==v.point)
			{
				Vertex* v_temp = findVertex(temp_shape->getEndPoint());
				if(v_temp!=NULL)
					vertex_list.push_back(v_temp);
			}
			if(temp_shape->getEndPoint()==v.point)
			{
				Vertex* v_temp = findVertex(temp_shape->getStartPoint());
				if(v_temp!=NULL)
					vertex_list.push_back(v_temp);
			}
		}
	}
}

bool Graph::removeShape(const Shape& s)
{
	bool ret=false;
	Shape* s_temp = findShape(s);
	if(s_temp)
	{
		ret = true;
		shapes.remove(s_temp);
	}
	return ret;
}

std::ostream& Graph::getCncCode(std::ostream &os, unsigned int& jump_mark)
{
	// z move is done here
	// first fast Move to 0.0
	os << "G0" << " Z" << 0.0 << std::endl;
	int count=(int)(fabs(m_property->getCutDepth())/fabs(m_property->getZPitch()));
	double last_z_pitch = fmod(fabs(m_property->getCutDepth()), fabs(m_property->getZPitch()));
	double signed_z_pitch;
	if(m_property->getCutDepth()<0)
		signed_z_pitch=fabs(m_property->getZPitch());
	else
		signed_z_pitch=-fabs(m_property->getZPitch());
	if(last_z_pitch>0.0)
		++count;	
	double actual_z_pos=m_property->getZStartPosition();
	for(int i=0; i<count;++i)
	{
		if(i==(count-1))
			actual_z_pos=m_property->getZStartPosition()-m_property->getCutDepth(); //last position
		else
			actual_z_pos+=signed_z_pitch;
		// slow move to Z-Position with Z FEED RATE
		if(m_property->getZFeedRate() != 0.0)
			os << "F" << m_property->getZFeedRate() << std::endl; //Z Feed Rate
		os << "G1" << " Z" << actual_z_pos << std::endl; // next Z Position of the canned cycle
		// Restore Cut FEED RATE
		if(m_property->getCutFeedRate() != 0.0)
			os << "F" << m_property->getCutFeedRate() << std::endl; //Cutter Feed Rate	
		for(cItShapeList it=shapes.begin(); it!=shapes.end(); ++it)
		{
			Shape* s_temp=*it;
			assert(s_temp);
			s_temp->getCncCode(os, jump_mark);
		}
		if(i<(count-1)  && shapes.size() > 0)
		{
			// create connection move back to start position
			//up
			os << "G0" << " Z" << m_connection_property->getZStartPosition() << std::endl;
			// to start Point2D
			os << "G0" << " X" << getStartPoint().x << " Y" << getStartPoint().y << std::endl;
			//down
			os << "G0" << " Z" << actual_z_pos << std::endl;
		}
	}
	return os;
}

bool Graph::isCircle()
{
	// since just cycles could be a circle 
	return false;
}

Point2D Graph::getGravityPoint()
{
	// just a simple gravity callculation by x and y avrerage
	assert(vertices.size()==shapes.size());
	double count=0.0;
	double x=0.0;
	double y=0.0;
	
	for(cItVertexList it_v=vertices.begin(); it_v!=vertices.end(); ++it_v)
	{
		count+=1.0;
		Vertex* v=*it_v;
		assert(v);
		x+=v->point.x;
		y+=v->point.y;
	}
	Point2D ret;
	ret.x=x/count;
	ret.y=y/count;
	return ret;
}

Vertex* Graph::extract_min_distance()
{
	Vertex* u = NULL;
	if(!vertices.empty())
	{
			
		for(cItVertexList it=vertices.begin(); it!=vertices.end(); it++)
		{
			if(u==NULL && *it!=NULL && (*it)->visited==false)
				u=*it;
			if(u!=NULL && *it!=NULL && (*it)->visited==false)
			{
				Vertex* v = *it;
				if(v->dist < u->dist)
				{
					u = v;
				}
			}
		}
	}
	if(u != NULL)
	{
		u->visited=true;
	}
	return u;
}

void Graph::createAllVertices()
{
	cItShapeList it;
	for(it=shapes.begin(); it!=shapes.end(); it++)
	{
		Shape* temp_shape = *it;
		if(temp_shape!=NULL)
		{
			//std::cout << temp_shape->print() << std::endl;
			if(findVertex(temp_shape->getStartPoint())== NULL)
			{
				Vertex* v = new Vertex;
				v->point=temp_shape->getStartPoint();
				vertices.push_back(v);
			}
			if(findVertex(temp_shape->getEndPoint())== NULL)
			{
				Vertex* v = new Vertex();
				v->point= temp_shape->getEndPoint();
				vertices.push_back(v);
			}
		}
		
	}	
}

void Graph::createAllPossibleConnections()
{
	for(cItVertexList it_i=vertices.begin(); it_i!=vertices.end(); ++it_i)
	{
		Vertex* v_i=*it_i;
		assert(v_i);
		for(cItVertexList it_j=vertices.begin(); it_j!=vertices.end(); ++it_j)
		{
			Vertex* v_j=*it_j;
			assert(v_j);
			if(v_i->point != v_j->point)
			{
				Connection l_temp(v_i->point, v_j->point);
				addShapeIfNotIn(l_temp);
			}				
		}
	}
}

LayerProperty* const Graph::getProperty()const
{ 
	return m_property;
}

void Graph::setProperty(LayerProperty* const lp)
{
	m_property=lp;
	for(cItShapeList it=shapes.begin(); it!=shapes.end(); ++it)
	{
		Shape* s_temp=*it;
		assert(s_temp);
		s_temp->setProperty(lp);
	}
}

LayerProperty* const Graph::getConnectionProperty()const
{
	return m_connection_property;
}

void Graph::setConnectionProperty(LayerProperty* const lp)
{
	m_connection_property=lp;
	for(cItShapeList it=shapes.begin(); it!=shapes.end(); ++it)
	{
		Shape* s_temp=*it;
		assert(s_temp);
		s_temp->setConnectionProperty(lp);
	}
}

void Graph::getMaxClippingArea(Point2D& p_lowleft, Point2D& p_upright)
{
	for(cItShapeList it=this->shapes.begin(); it!=this->shapes.end(); ++it)
	{
		Shape* temp_shape = *it;
		if(temp_shape!=NULL)
		{
			temp_shape->getMaxClippingArea(p_lowleft,p_upright);
		}
	}	
}

void Graph::draw(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)
{
	assert(m_property);
	for(cItShapeList it=this->shapes.begin(); it!=this->shapes.end(); ++it)
	{
		Shape* temp_shape = *it;
		if(temp_shape!=NULL)
		{
			temp_shape->draw(win, scale, offset);
			if(it==this->shapes.begin())
				temp_shape->drawStartBar(win, scale, offset);
			if(it==(--(this->shapes.end())))
				temp_shape->drawEndArrow(win, scale, offset);
		}
	}	
}

Graph& Graph::intersectionSet(const Graph& g)
{
	for(itVertexList it=vertices.begin();it!=vertices.end();it++)
	{
		Vertex* v_temp=*it;
		assert(v_temp);
		if(!g.findVertex(*v_temp))
		{
			vertices.erase(it);
			delete v_temp;
			--it;
		}
	}
	for(ItShapeList it=shapes.begin();it!=shapes.end();it++)
	{
		Shape* s_temp=*it;
		assert(s_temp);
		if(!g.findShape(*s_temp))
		{
			shapes.erase(it);
			delete s_temp;
			--it;
		}
	}
	return *this;
}

Graph& Graph::unionSet(const Graph& g)
{
	for(cItShapeList it_s=g.shapes.begin(); it_s!=g.shapes.end(); it_s++)
		addShapeIfNotIn(**it_s);
	for(cItVertexList it_v=g.vertices.begin(); it_v!=g.vertices.end(); it_v++)
		addVertexIfNotIn(**it_v);
	return *this;
}

Graph& Graph::differencSet(const Graph& g)
{
	for(cItVertexList it=g.vertices.begin();it!=g.vertices.end();it++)
	{
		Vertex* v_temp_g=*it;
		assert(v_temp_g);
		Vertex* v_temp=findVertex(v_temp_g->point);
		if(v_temp!=NULL)
		{
			vertices.remove(v_temp);
			delete v_temp;
		}
		else
		{
			addVertexIfNotIn(*v_temp_g);
		}
	}
	for(cItShapeList it=g.shapes.begin();it!=g.shapes.end();it++)
	{
		Shape* s_temp_g=*it;
		assert(s_temp_g);
		Shape* s_temp=findShape(*s_temp_g);
		if(s_temp!=NULL)
		{
			shapes.remove(s_temp);
			delete s_temp;
		}
		else
		{
			addShapeIfNotIn(*s_temp_g);
		}
	}
	return *this;

}

Graph& Graph::uniDirectDifferencSet(const Graph& g)
{
	for(cItVertexList it=g.vertices.begin();it!=g.vertices.end();it++)
	{
		Vertex* v_temp_g=*it;
		assert(v_temp_g);
		Vertex* v_temp=findVertex(v_temp_g->point);
		if(v_temp!=NULL)
		{
			vertices.remove(v_temp);
			delete v_temp;
			////--it;
		}
		else
		{
			addVertexIfNotIn(*v_temp_g);
		}
	}
	for(cItShapeList it=g.shapes.begin();it!=g.shapes.end();it++)
	{
		Shape* s_temp_g=*it;
		assert(s_temp_g);
		Shape* s_swap_g=s_temp_g->clone();
		s_swap_g->swapDirection();
		
		Shape* s_temp=findShape(*s_temp_g);
		Shape* s_swap=findShape(*s_swap_g);
		
		if(s_temp!=NULL || s_swap!=NULL)
		{
			if(s_temp!=NULL)
			{
				shapes.remove(s_temp);
				delete s_temp;
			}
			if(s_swap!=NULL && g.findShape(*s_swap_g)==NULL)
			{
				shapes.remove(s_swap);
				delete s_swap;
			}
		}
		else
		{
			addShapeIfNotIn(*s_temp_g);
		}
		if(s_swap_g!=NULL)
		{
			delete s_swap_g;
			s_swap_g=NULL;
		}
	}
	return *this;
}

float Graph::getLength() const
{
	float length = 0;
	for(cItShapeList it=shapes.begin(); it!=shapes.end(); it++)
	{
		Shape* temp_shape= *it;
		if(temp_shape!=NULL)
			length += temp_shape->getLength();
	}
	return length;
}

bool Graph::operator==(const Graph& g)const
{
	//bool ret = vertices == g.vertices && shapes==g.shapes; 
	bool ret=false;
	if( shapes.size()==g.shapes.size() && vertices.size()==g.vertices.size())
	{
		ret=true;
		cItShapeList it_s=shapes.begin();
		cItShapeList it_s_g=g.shapes.begin();
		for(; it_s!=shapes.end(); it_s++)
		{
			Shape* s = *it_s;
			assert(s);
			Shape* s_g=*it_s_g;
			assert(s_g);
			if(*s!=*s_g)
			{
				ret=false;
				break;
			}
			++it_s_g;
		}
		if(ret==true)
		{
			cItVertexList it_v=vertices.begin();
			cItVertexList it_v_g=g.vertices.begin();
			for(; it_v!=vertices.end(); it_v++)
			{
				Vertex* v = *it_v;
				assert(v);
				Vertex* v_g=*it_v_g;
				assert(v_g);
				if(v->point!=v_g->point)
				{
					ret=false;
					break;
				}
				++it_v_g;
			}
		}
	}
	return ret;
}

bool Graph::operator!=(const Graph& g)const
{
	
	return !operator==(g);
}

void Graph::dijkstra(Vertex& v_source, const Vertex& v_target, Graph& g_shortes_path_list)
{
	
	g_shortes_path_list.clear();
	Vertex* v;
	for(cItVertexList it_v=vertices.begin();it_v!=vertices.end(); it_v++)
	{ 
		v=*it_v;
		assert(v);
		if(v!=NULL)
		{
			v->dist=INFINITY;
			v->previous=NULL;
			v->connection=NULL;
			v->visited=false;
		}
	}
	v=findVertex(v_source.point);
	v->dist=0;
	v=NULL;
	Vertex* u = NULL;
	while((u = extract_min_distance())!=NULL)
	{
		if( *u==v_target)
			break;
		if(u->dist==INFINITY)
			continue;
		lShapeList connected_shapes;
		getAllConncectedShapes(connected_shapes, *u);
		for(cItShapeList itS=connected_shapes.begin(); itS!=connected_shapes.end(); itS++)
		{
			Shape* temp_shape = *itS;
			if(temp_shape->getEndPoint() != u->point)
			{
				v = findVertex(temp_shape->getEndPoint());
			}
			else 
			{
				v = findVertex(temp_shape->getStartPoint());
			}
			double alt = u->dist + temp_shape->getLength();
			if(alt < v->dist )
			{
				assert(alt<INFINITY);
				v->dist = alt;
				v->previous = u;
				v->connection = temp_shape;
			}
		}
	}
	Vertex* pre=u;
	while(pre!=NULL)
	{
		g_shortes_path_list.addVertexIfNotIn(*pre);
		if(pre->connection!=NULL)
		{
			g_shortes_path_list.addShapeIfNotIn(*(pre->connection));
		}
		pre=pre->previous;
	}
	//g_shortes_path_list is a return value
}

void Graph::dijkstraMaxDist(Vertex& v_source, const Vertex& v_target, Graph& g_longest_path_list)
{
	
	g_longest_path_list.clear();
	Vertex* v;
	for(cItVertexList it_v=vertices.begin();it_v!=vertices.end(); it_v++)
	{ 
		v=*it_v;
		assert(v);
		if(v!=NULL)
		{
			v->dist=INFINITY;
			v->previous=NULL;
			v->connection=NULL;
			v->visited=false;
		}
	}
	v=findVertex(v_source.point);
	v->dist=0;
	v=NULL;
	Vertex* u = NULL;
	while((u = extract_min_distance())!=NULL)
	{
		if( *u==v_target)
			break;
		if(u->dist==INFINITY)
			continue;
		////std::cout << "dijkstraMaxDist extract_max_distance" << std::endl;
		lShapeList connected_shapes;
		getAllConncectedShapes(connected_shapes, *u);
		for(cItShapeList itS=connected_shapes.begin(); itS!=connected_shapes.end(); itS++)
		{
			Shape* temp_shape = *itS;
			assert(temp_shape);
			
			if(temp_shape->getStartPoint() == u->point)
			{
				v = findVertex(temp_shape->getEndPoint());
				double alt = u->dist - temp_shape->getLength();
				//std::cout << "dijkstraMaxDist connected_shapes" << std::endl;
				if(alt < v->dist )
				{
					///std::cout << "dijkstraMaxDist alt > v->dist" << std::endl;
					assert(alt<INFINITY);
					v->dist = alt;
					v->previous = u;
					v->connection = temp_shape;
				}
			}
		}
	}
	Vertex* pre=u;
	while(pre!=NULL)
	{
		g_longest_path_list.addVertexIfNotIn(*pre, true);
		if(pre->connection!=NULL)
		{
			g_longest_path_list.addShapeIfNotIn(*(pre->connection), true);
		}
		pre=pre->previous;
	}
	//g_shortes_path_list is a return value
}


void Graph::sortCounterClockWise()
{
	//nothing to do on a simple graph
	std::cout << "Graph::sortCounterClockWise: Nothing to do!" << std::endl;
}

void Graph::sortClockWise()
{
	//nothing to do on a simple graph
}

void Graph::sortShapesByLength()
{
	shapes.sort(compareShapeLength);
}

Point2D Graph::getStartPoint()
{
	Shape* s_front=this->shapes.front();
	assert (s_front);
	Point2D s_point=s_front->getStartPoint();
	
	return s_point;
	//return findVertex(this->shapes.front()->getStartPoint());
}
Point2D Graph::getEndPoint()
{
	Shape* s_back=this->shapes.back();
	assert (s_back);
	Point2D e_point=s_back->getEndPoint();
	return e_point;
}

bool Graph::compareShapeLength(Shape* first, Shape* second)
{
	bool ret = false;
	if ( first!=NULL && second!=NULL && first->getLength() <  second->getLength()) 
		ret=true;
	return ret;
}
