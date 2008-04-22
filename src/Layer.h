/*
	File: Layer.h
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

#ifndef _LAYER_H_
#define _LAYER_H_

#include <iostream>
#include <sstream>
#include <string>
#include "Shape.h"
#include "Graph.h"
#include "GraphList.h"
#include "LayerProperty.h"
#include "ConnectionProperty.h"
#include "ConfigSettings.h"

#define NC_VAR_TOOL1_LENGTH 2000
#define NC_VAR_TOOL_ACTUAL_LENGTH 2001
#define NC_VAR_TOOL_CHANGE_X 2011
#define NC_VAR_TOOL_CHANGE_Y 2012
#define NC_VAR_TOOL_CHANGE_Z 2013
#define NC_VAR_TOOL_SENSOR_X 2021
#define NC_VAR_TOOL_SENSOR_Y 2022
#define NC_VAR_TOOL_SENSOR_Z 2023
#define NC_VAR_TOOL_SENSOR_HEIGHT 2024
#define NC_VAR_TOOL_SENSOR_TOUCH_OFF_HEIGHT 2025
#define NC_VAR_EXIT_X 2031
#define NC_VAR_EXIT_Y 2032
#define NC_VAR_EXIT_Z 2033

typedef std::list<GraphList*> lGlList;
typedef std::list<GraphList*>::iterator itGlList;
typedef std::list<GraphList*>::const_iterator cItGlList;


class Layer 
{
	public:
		Layer(const std::string& layer_name);
		virtual ~Layer();
		virtual std::string getName();
		virtual void addShape(const Shape& shape);
		const Graph& getInputGraph();
		void calcGraphs(void);
		virtual void getMaxClippingArea(Point2D& p_lowleft, Point2D& p_upright);
		virtual void draw(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset);
		std::string print(void);
		void setConfigSettings(ConfigSettings* const cs);
		ConfigSettings* const getConfigSettings() const;
		LayerProperty* const getProperty()const;
		void setProperty(LayerProperty* const lp);
		LayerProperty* const getConnectionProperty()const;
		void setConnectionProperty(LayerProperty* const cp);
		virtual std::ostream& getCncCode(std::ostream &os, unsigned int& jump_mark, bool tool_change=true, bool first_layer=false);
		
	protected:
		void graphDeepFirstSearch(/*out*/GraphList& all_graphs);
		void visitGraphDeepFirstSearch(/*in,out*/Graph& graph, Vertex& v);
		void calcMinimumCycleBasis(Graph& g, /*out*/GraphList& g_select_cycles);
		void calcEssentialCycles(const GraphList& gl_mcb, /*out*/GraphList& gl_J);
		void calcGraphNoCycles(Graph& g_all, GraphList& g_all_cycles, /*out*/GraphList& gl_no_cycle);
		void calcConnections(/*out*/GraphList& gl_connections);
		//
		std::string m_name;
		Graph m_input_graph;
		GraphList m_all_cycles;
		GraphList m_all_not_cycles;
		GraphList m_all_graphs;
		ConfigSettings* m_config_settings;
		LayerProperty* m_property;
		LayerProperty* m_connection_property;
};


#endif	//_LAYER_H_
