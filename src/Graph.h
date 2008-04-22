/*
	File: Graph.h
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

#ifndef _GRAPH_H_
#define _GRAPH_H_
#include "Shape.h"
#include "Vertex.h"
#include "LayerProperty.h"
#include <list>
#include <string>
#include <iostream>
#include <sstream>


typedef std::list<Shape*> lShapeList;
typedef std::list<Shape*>::const_iterator cItShapeList;
typedef std::list<Shape*>::iterator ItShapeList;
typedef std::list<Vertex*> lVertexList;
typedef std::list<Vertex*>::const_iterator cItVertexList;
typedef std::list<Vertex*>::iterator itVertexList;

class Graph
{
	public:
		Graph();
		Graph(const Graph& g);
		Graph(const lShapeList& s);
		virtual ~Graph();
		virtual Graph* clone() const;
		void clear();
		lShapeList shapes; 
		lVertexList vertices;
		std::string print();
		std::string printExt();
		bool addShapeIfNotIn(const Shape& s, bool front=false);
		bool addVertexIfNotIn(const Vertex& v, bool front=false);
		Vertex* findVertex(const Point2D p) const;
		bool findVertex(const Vertex& v) const;
		Shape* findShape(const Shape& s) const;
		void getAllConncectedShapes(lShapeList& shape_list, const Vertex& v);
		void getAllConncectedVertices(lVertexList& vertex_list, const Vertex& v);
		virtual bool removeShape(const Shape& s);
		// schnittmenge zwei graphen 
		Graph& intersectionSet(const Graph& g);
		// Vereinigungsmenge
		Graph& unionSet(const Graph& g);
		// differenz menge
		Graph& differencSet(const Graph& g);
		Graph& uniDirectDifferencSet(const Graph& g);
		float getLength() const;
		bool operator==(const Graph& g)const;
		bool operator!=(const Graph& g)const;
		void dijkstra(Vertex& v_source, const Vertex& v_target, Graph& g_shortes_path_list);
		void dijkstraMaxDist(Vertex& v_source, const Vertex& v_target, Graph& g_longest_path_list);
		virtual void sortCounterClockWise();
		virtual void sortClockWise();
		virtual void sortShapesByLength();
		virtual void createAllVertices();
		virtual void getMaxClippingArea(Point2D& p_lowleft, Point2D& p_upright);
		virtual void draw(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset);
		virtual Point2D getStartPoint();
		virtual Point2D getEndPoint();
		virtual void createAllPossibleConnections();
		virtual LayerProperty* const getProperty()const;
		virtual void setProperty(LayerProperty* const lp);
		virtual LayerProperty* const getConnectionProperty()const;
		virtual void setConnectionProperty(LayerProperty* const cp);
		virtual std::ostream& getCncCode(std::ostream &os, unsigned int& jump_mark);
		virtual bool isCircle();
		virtual Point2D getGravityPoint();
	protected:
		Vertex* extract_min_distance();
		void clearVertices();
		void clearShapes();
		static bool compareShapeLength(Shape* first, Shape* second);
		LayerProperty* m_property;
		LayerProperty* m_connection_property;		
};


#endif	//_GRAPH_H_
