/*
	File: GraphList.h
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

#ifndef _GRAPHLIST_H_
#define _GRAPHLIST_H_
#include "Graph.h"
#include <list>
#include <string>
#include <iostream>
#include <sstream>


typedef std::list<Graph*> lGraphList;
typedef std::list<Graph*>::iterator itGraphList;
typedef std::list<Graph*>::const_iterator cItGraphList;
typedef std::list<Graph*>::const_reverse_iterator cRitGraphList;
class GraphList
{
	public:
		GraphList();
		GraphList(const GraphList& glist);
		virtual ~GraphList();
		GraphList& operator=(const GraphList& glist);
		bool operator==(const GraphList& glist)const;
		bool operator!=(const GraphList& glist)const;
		Graph* find(const Graph& g) const;
		Graph* findCycle(const Graph& g) const;
		bool addGraphIfNotIn(const Graph& g);
		bool addGraphIfCycleNotIn(const Graph& g);
		bool remove(const Graph& g);
		void clear(void);
		std::string print(void);
		void sortByCycleLenght(void);
		// schnittmenge zwei graphen 
		GraphList& intersectionSet(const GraphList& gl);
		// Vereinigungsmenge
		GraphList& unionSet(const GraphList& gl);
		// rank the linear indent cycels
		unsigned int rank();
		virtual void sortCounterClockWise(void);
		virtual void sortClockWise(void);
		virtual void getMaxClippingArea(Point2D& p_lowleft, Point2D& p_upright);
		virtual void draw(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset);
		virtual LayerProperty* const getProperty()const;
		virtual void setProperty(LayerProperty* const lp);
		virtual LayerProperty* const getConnectionProperty()const;
		virtual void setConnectionProperty(LayerProperty* const cp);
		virtual std::ostream& getCncCode(std::ostream &os, unsigned int& jump_mark);
		
		//
		unsigned int verticesCount();
		lGraphList graphs;
		
	private:
		static bool compareCycleLength(Graph* first, Graph* second);
		LayerProperty* m_property;
		LayerProperty* m_connection_property;
	
};


#endif	//_GRAPLIST_H_
