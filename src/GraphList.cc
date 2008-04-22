/*
	File: GraphList.cc
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

#include "GraphList.h"
#include "GraphListVerticesBits.h"
#include <assert.h>

GraphList::GraphList()
{
	// TODO: put constructor code here
}

GraphList::GraphList(const GraphList& glist)
{
	for(cItGraphList it=glist.graphs.begin(); it!=glist.graphs.end(); it++)
	{
		Graph* g_temp = *it;
		if( g_temp!=NULL)
		{
			graphs.push_back(g_temp->clone());
		}
	}
}

GraphList::~GraphList()
{
	clear();
}

GraphList& GraphList::operator=(const GraphList& glist)
{
	clear();
	for(cItGraphList it=glist.graphs.begin(); it!=glist.graphs.end(); it++)
	{
		Graph* g_temp = *it;
		if( g_temp!=NULL)
		{
			graphs.push_back(g_temp->clone());
		}
	}
	return *this;
}

bool GraphList::operator==(const GraphList& glist)const
{
	//bool ret = vertices == g.vertices && shapes==g.shapes; 
	bool ret=false;
	if(graphs.size() == glist.graphs.size())
	{
		ret = true;
		cItGraphList it=graphs.begin();
		cItGraphList it_gl=glist.graphs.begin();
		for(; it!=graphs.end(); it++)
		{
			Graph* g = *it;
			assert(g);
			Graph* g_gl=*it_gl;
			assert(g_gl);
			if(*g!=*g_gl)
			{
				ret=false;
				break;
			}
			++it_gl;
		}
	}
	return ret;
}

bool GraphList::operator!=(const GraphList& glist)const
{
	
	return !operator==(glist);
}


void GraphList::clear(void)
{
	for(cItGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		Graph* g_temp=*it;
		if(g_temp!=NULL)
		{
			delete g_temp;
			g_temp=NULL;
		}
	}
	graphs.clear();	
}

std::string GraphList::print(void)
{
	std::stringstream stream;
	for(cItGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		Graph* g_temp=*it;
		if(g_temp!=NULL)
		{
			stream << "----" << std::endl;
			stream << "cycle: length" << g_temp->getLength() << g_temp->print();
		}
	}
	return stream.str();
}

Graph* GraphList::find(const Graph& g) const
{
	Graph* ret =NULL;	
	for(cItGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		Graph* g_temp=*it;
		assert(g_temp);
		if(*g_temp==g)
		{			
			ret=g_temp;
			break;
		}
	}
	return ret;
}

Graph* GraphList::findCycle(const Graph& g) const
{
	Graph* ret =NULL;
	if(g.vertices.size()>0)
	{
		for(cItGraphList it=graphs.begin(); it!=graphs.end(); it++)
		{
			if(*it!=NULL)
			{			
				Graph g_intersect(**it);
				g_intersect.intersectionSet(g);
				if(g_intersect==**it)
				{
					ret=*it;
					break;
				}
			}
		}
	}
	return ret;
}

bool GraphList::addGraphIfNotIn(const Graph& g)
{
	bool ret=false;	
	if(find(g)==NULL)
	{
		graphs.push_back(g.clone());
		ret=true;
	}
	return ret;
}

bool GraphList::addGraphIfCycleNotIn(const Graph& g)
{
	bool ret= false;
	if(findCycle(g)==NULL)
	{
		graphs.push_back(g.clone());
		ret = true;
	}
	return ret;
}

bool GraphList::remove(const Graph& g)
{
	bool ret =false;
	for(itGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		Graph* g_temp = *it;
		assert(g_temp);
		if(g_temp!=NULL && *g_temp==g);
		{			
			ret=true;
			delete g_temp;
			graphs.erase(it);
			break;
		}
	}
	return ret;
}

void GraphList::sortByCycleLenght(void)
{
	graphs.sort(compareCycleLength);
}

bool GraphList::compareCycleLength(Graph* first, Graph* second)
{
	bool ret = false;
	if ( first!=NULL && second!=NULL && first->getLength() <  second->getLength()) 
		ret=true;
	return ret;
}


GraphList& GraphList::intersectionSet(const GraphList& gl)
{
	for(itGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		Graph* g_temp=*it;
		assert(g_temp);
		if(!gl.find(*g_temp))
		{
			delete(g_temp);
			graphs.erase(it);
			--it;
		}
	}
	return *this;
}

GraphList& GraphList::unionSet(const GraphList& gl)
{
	for(cItGraphList it=gl.graphs.begin(); it!=gl.graphs.end(); it++)
	{
		Graph* g_temp=*it;
		if(g_temp!=NULL)
			addGraphIfNotIn(*g_temp);
	}
	return *this;
}

unsigned int GraphList::rank()
{
	GraphListVerticesBits bits(graphs);
	return bits.rank();
}

unsigned int GraphList::verticesCount()
{
	Graph all_union_set;
	for(cItGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		Graph* g_temp=*it;
		if(g_temp!=NULL)
			all_union_set.unionSet(*g_temp);
	}
	return all_union_set.vertices.size();
}

void GraphList::sortCounterClockWise(void)
{
	for(cItGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		Graph* g_temp=*it;
		if(g_temp!=NULL)
			g_temp->sortCounterClockWise();
	}
}

void GraphList::sortClockWise(void)
{
	for(cItGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		Graph* g_temp=*it;
		if(g_temp!=NULL)
			g_temp->sortClockWise();
	}
}

void GraphList::getMaxClippingArea(Point2D& p_lowleft, Point2D& p_upright)
{
	for(cItGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		Graph* g_temp=*it;
		if(g_temp!=NULL)
			g_temp->getMaxClippingArea(p_lowleft, p_upright);
	}
}

void GraphList::draw(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)
{
	for(cItGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		Graph* g_temp=*it;
		if(g_temp!=NULL)
			g_temp->draw(win, scale, offset);
	}
}

LayerProperty* const GraphList::getProperty()const
{ 
	return m_property;
}

void GraphList::setProperty(LayerProperty* const lp)
{
	m_property=lp;
	for(cItGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		Graph* g_temp=*it;
		if(g_temp!=NULL)
			g_temp->setProperty(lp);
	}
}

LayerProperty* const GraphList::getConnectionProperty()const
{
	return m_connection_property;
}

void GraphList::setConnectionProperty(LayerProperty* const lp)
{
	m_connection_property=lp;
	for(cItGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		Graph* g_temp=*it;
		if(g_temp!=NULL)
			g_temp->setConnectionProperty(lp);
	}
}

std::ostream& GraphList::getCncCode(std::ostream &os, unsigned int& jump_mark)
{
	for(cItGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		Graph* g_temp=*it;
		if(g_temp!=NULL)
			g_temp->getCncCode(os, jump_mark);
	}
	return os;
}
