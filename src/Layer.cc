/*
	File: Layer.cc
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

#include "Layer.h"
#include <assert.h>
#include <math.h>
#include "Arc.h"
#include "Cycle.h"
#include "ConnectionGraph.h"
#include <iostream>
#include <sstream>

Layer::Layer(const std::string& layer_name)
{
	m_name=layer_name;
	m_property=NULL;
}


Layer::~Layer()
{
	// TODO: put destructor code here
}

std::string Layer::getName()
{
	return m_name;
}

void Layer::addShape(const Shape& shape)
{
	m_input_graph.addShapeIfNotIn(shape);
}

const Graph& Layer::getInputGraph()
{
	return m_input_graph;
}
void Layer::calcGraphs(void)
{
	if(m_property && m_connection_property)
	{
		m_input_graph.createAllVertices();
		m_all_cycles.clear();
		m_all_not_cycles.clear();
		////std::cout << input_graph.print() << std::endl;
		// Deep first search
		GraphList all_connected_graphs;
		graphDeepFirstSearch(all_connected_graphs);
		for(cItGraphList itG = all_connected_graphs.graphs.begin(); itG != all_connected_graphs.graphs.end(); itG++)
		{
			Graph* temp_graph = *itG;
			assert(temp_graph!=NULL);
			////std::cout << "----------tempGraph----------" << std::endl;
			////std::cout << temp_graph->print() << std::endl;
			GraphList cycleList;
			calcMinimumCycleBasis(*temp_graph, cycleList);
			////std::cout << "----------calcMinimumCycleBasis---" << std::endl;
			////std::cout << cycleList.print() << std::endl;
			GraphList gl_essential;
			std::cout << "----------calcEssentialCycles----------" << std::endl;
			calcEssentialCycles(cycleList, gl_essential);
			if(m_property)
			{
				if(m_property->getCutDirection()==CCW)
				{
					gl_essential.sortCounterClockWise();
					std::cout << "Sort CCW" << std::endl;
				}
				else
				{
					gl_essential.sortClockWise();
					std::cout << "Sort CW" << std::endl;
				}
			}
			////std::cout << gl_essential.print() << std::endl;
			m_all_cycles.unionSet(gl_essential);
			
		}	
		
		GraphList gl_no_cycle;
		calcGraphNoCycles(m_input_graph, m_all_cycles, m_all_not_cycles);
		std::cout << "----------All Shapes not in Cycle----------" << std::endl;
		std::cout << m_all_not_cycles.print() << std::endl;
		//
		calcConnections(m_all_graphs);
		m_all_graphs.setProperty(m_property);
		m_all_graphs.setConnectionProperty(m_connection_property);
	}
}

std::string Layer::print(void)
{
	std::stringstream stream;
	stream << std::endl << "Layer:" << m_name << std::endl;
	stream << m_all_cycles.print() << std::endl;
	return stream.str();
}


void Layer::getMaxClippingArea(Point2D& p_lowleft, Point2D& p_upright)
{
	for(cItGraphList it_a = m_all_cycles.graphs.begin(); it_a!=m_all_cycles.graphs.end(); ++it_a)
	{
		Graph* t_temp=*it_a;
		assert(t_temp);
		if(t_temp!=NULL)
			t_temp->getMaxClippingArea(p_lowleft, p_upright);
	}
	for(cItGraphList it_n = m_all_not_cycles.graphs.begin(); it_n!=m_all_not_cycles.graphs.end(); ++it_n)
	{
		Graph* t_temp=*it_n;
		assert(t_temp);
		if(t_temp!=NULL)
			t_temp->getMaxClippingArea(p_lowleft, p_upright);
	}
}

void Layer::draw(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)
{
	if(m_property && m_connection_property)
	{
		for(cItGraphList it_a = m_all_graphs.graphs.begin(); it_a!=m_all_graphs.graphs.end(); ++it_a)
		{
			Graph* t_temp=*it_a;
			assert(t_temp);
			if(t_temp!=NULL)
				t_temp->draw(win, scale, offset);
		}
	}
}

std::ostream& Layer::getCncCode(std::ostream &os, unsigned int& jump_mark, bool tool_change, bool first_layer)
{
	if(m_property && m_connection_property)
	{
		if(!(m_property->getName().empty()))
			os << "(LAYER: " << m_property->getName() << ")" << std::endl;
		//tool change
		if(tool_change)
		{
			if(m_config_settings->getAddToolChangePos())
			{
				os << "G0" << " G53" << " Z" << "#" << NC_VAR_TOOL_CHANGE_Z  
					<< " (Go absolut to Tool Z CHANGE position)" << std::endl;
				os << "G0" << " G53" << " X" << "#" << NC_VAR_TOOL_CHANGE_X  
					<< " Y" << "#" << NC_VAR_TOOL_CHANGE_Y  
					<< " (Go absolut to Tool CHANGE position)" << std::endl;
			}
			os << "M6" << " T" << m_property->getToolNumber() << std::endl;
		
			if(m_config_settings->getUseToolSensor())
			{
				os << "G0" << " G53" << " X" << "#" << NC_VAR_TOOL_SENSOR_X  
					<< " Y" << "#" << NC_VAR_TOOL_SENSOR_Y  
					<< " (Go absolut to Tool Sensor start probe position)" << std::endl;
				os << "G49" << " (Measure without last tool offset)" <<std::endl;
				os << "F" << m_config_settings->getToolSensorFeedRate() << " (Tool Sensor Fedd Rate)" << std::endl; 
				os << "G38.2" << " Z" << m_config_settings->getToolSensorZPos() << std::endl;
				if(first_layer)
				{
					// save 1st tool length
					os << "#" << NC_VAR_TOOL1_LENGTH << "=" << "#5063" << " (Save 1st tool length)" << std::endl;
					if(m_config_settings->getAddToolChangePos())
					{					
						//go back to tool change position
						os << "G0" << " G53" << " Z" << "#" << NC_VAR_TOOL_CHANGE_Z  
						<< " (Go absolut back to Tool Z CHANGE position)" << std::endl;
						os << "G0" << " G53" << " X" << "#" << NC_VAR_TOOL_CHANGE_X  
						<< " Y" << "#" << NC_VAR_TOOL_CHANGE_Y  
						<< " (Go absolut back to Tool CHANGE position)" << std::endl;
					}
				}
				else
				{
					// save 1st tool length
					os << "#" << NC_VAR_TOOL_ACTUAL_LENGTH << "=" << "#5063" << " (Save actual tool length)" << std::endl;
					if(m_config_settings->getAddToolChangePos())
					{					
						//go back to tool change position
						os << "G0" << " G53" << " Z" << "#" << NC_VAR_TOOL_CHANGE_Z  
						<< " (Go absolut back to Tool Z CHANGE position)" << std::endl;
						os << "G0" << " G53" << " X" << "#" << NC_VAR_TOOL_CHANGE_X  
						<< " Y" << "#" << NC_VAR_TOOL_CHANGE_Y  
						<< " (Go absolut back to Tool CHANGE position)" << std::endl;
					}
					// set new tool offset
					os << "G43.1" << " K" << "[#" << NC_VAR_TOOL_ACTUAL_LENGTH << "-#" <<  NC_VAR_TOOL1_LENGTH << "]" 
						<< " (set new tool offset)" << std::endl;
				}
				os << "G90" << " (Switch back to absolute ditance mode)" <<std::endl;
			}
		}
		if(m_property->getCoolingMist())
		{	
			os << "M7" << std::endl; 	// turn mist cooling on
			if(m_property->getCoolingFlood())
				os << "M8" << std::endl; 	// turn flood cooling on
		}
		else
			os << "M9" << std::endl;	// turn cooling off		
		if(m_property->getCutFeedRate() != 0.0)
			os << "F" << m_property->getCutFeedRate() << std::endl; //Cutter Feed Rate
		if(m_property->getSpindleSpeed() > 0.0)
		{
			os << "S" << m_property->getSpindleSpeed() << std::endl; // Spindle Speed
			os << "M3" << std::endl; // Switch Spindle On: cw
		}
		else if(m_property->getSpindleSpeed() < 0.0)
		{
			os << "S" << m_property->getSpindleSpeed() << std::endl; // Spindle Speed
			os << "M4" << std::endl; // Switch Spindle On: cw
		}
			
		
		
		
		for(cItGraphList it_a = m_all_graphs.graphs.begin(); it_a!=m_all_graphs.graphs.end(); ++it_a)
		{
			Graph* g_temp=*it_a;
			assert(g_temp);
			if(g_temp!=NULL && g_temp->getProperty()!=NULL)
			{
				if(it_a==m_all_graphs.graphs.begin())
				{
					//entry move
					Point2D p_start=g_temp->getStartPoint();
					os << "G0" << " X" << p_start.x << " Y" << p_start.y << std::endl;
					os << "G0" << " Z" << m_connection_property->getZStartPosition() << std::endl;
					
				}
				g_temp->getCncCode(os, jump_mark);
			}
				
		}
	}
	os << "G0" << " Z" << m_connection_property->getZStartPosition() << std::endl; // move up
	os << "M5" << std::endl; // Switch Spindle Off
	os << "M9" << std::endl;	// turn cooling off		
	return os;
}

void Layer::setConfigSettings(ConfigSettings* const cs)
{
	m_config_settings=cs;
}
	
ConfigSettings* const Layer::getConfigSettings() const
{
	return m_config_settings;
}
		

LayerProperty* const Layer::getProperty()const
{ 
	return m_property;
}
void Layer::setProperty(LayerProperty* const lp)
{
	m_property=lp;
	m_all_graphs.setProperty(lp);
}

LayerProperty* const Layer::getConnectionProperty()const
{ 
	return m_connection_property;
}
void Layer::setConnectionProperty(LayerProperty* const cp)
{
	m_connection_property=cp;
	m_all_graphs.setConnectionProperty(cp);
}

void Layer::graphDeepFirstSearch(/*out*/GraphList& all_connected_graphs)
{
	// first reset visit flag
	cItVertexList itV;
	for(itV=m_input_graph.vertices.begin(); itV!=m_input_graph.vertices.end(); itV++)
	{
		Vertex* v = *itV;
		v->visited=false;
		//std::cout << "here we go:" << std::endl;
	}
	for(itV=m_input_graph.vertices.begin(); itV!=m_input_graph.vertices.end(); itV++)
	{
		Vertex* v = *itV;
		if(!v->visited)
		{
			//we find a vertex of a new shape
			Graph new_graph;
			visitGraphDeepFirstSearch(new_graph, *v);
			// add to list of base_graphs
			all_connected_graphs.addGraphIfNotIn(new_graph);
		}
	}
}

void Layer::visitGraphDeepFirstSearch(/*in,out*/Graph& graph, Vertex& v)
{
	v.visited=true;
	//add to Graph
	graph.addVertexIfNotIn(v);
	//
	lShapeList shape_list;
	m_input_graph.getAllConncectedShapes(shape_list, v);
	cItShapeList itS;
	for(itS=shape_list.begin(); itS!=shape_list.end(); itS++)
	{
		Shape* temp_shape = *itS;
		assert(temp_shape!=NULL);
		// for start Point2D
		Vertex* temp_v = m_input_graph.findVertex(temp_shape->getStartPoint());
		if(temp_v!=NULL)
		{
			// temp_v == temp_shape.start_point   
			graph.addShapeIfNotIn(*temp_shape);
			if(!temp_v->visited)
			{
				//recursive call
				visitGraphDeepFirstSearch(graph, *temp_v);
			}	
		}
		// for end point
		temp_v = m_input_graph.findVertex(temp_shape->getEndPoint());
		assert(temp_v!=NULL);
		if(temp_v!=NULL)
		{
			// temp_v == temp_shape.end_point 
			graph.addShapeIfNotIn(*temp_shape);
			if(!temp_v->visited)
			{
				// recursive call
				visitGraphDeepFirstSearch(graph, *temp_v);
			}			
		}
	}
}

void Layer::calcMinimumCycleBasis(Graph& g, /*out*/GraphList& g_select_cycles)
{
	g_select_cycles.clear();
	for(cItVertexList it_v=g.vertices.begin(); it_v!=g.vertices.end(); it_v++)
	{
		Vertex* v=*it_v;
		assert(v);
		for(cItShapeList it_s=g.shapes.begin(); it_s!=g.shapes.end(); it_s++)
		{
			Shape* shape_xy = *it_s;
			assert(shape_xy);
			if(*v!=*g.findVertex(shape_xy->getStartPoint()) && *v!=*g.findVertex(shape_xy->getEndPoint()) )
			{
				Graph path_list_xv;
				g.dijkstra(*g.findVertex(shape_xy->getStartPoint()), *v, path_list_xv);
				
				Graph path_list_vy;
				g.dijkstra(*v, *g.findVertex(shape_xy->getEndPoint()), path_list_vy);	
				//itersection set
				Graph g_intersect(path_list_xv);
				g_intersect.intersectionSet(path_list_vy);
			
					
				if(g_intersect.vertices.size() == 1 && g_intersect.shapes.empty() && g_intersect.findVertex(*v))
				{
					////std::cout << "Here we go!+!" << std::endl;
					Cycle new_cycle(path_list_xv);
					new_cycle.unionSet(path_list_vy);
					new_cycle.addShapeIfNotIn(*shape_xy);
					bool added = g_select_cycles.addGraphIfCycleNotIn(new_cycle);
					if(added==false)
					{
						////std::cout << "!!!Graph not added:" << std::endl;
					}
					else
					{
						////std::cout << "addCycle:" << new_cycle.print() << std::endl;
						
					}
				}
			}
			else if(typeid(*shape_xy)==typeid(Arc))
			{
				////std::cout << "!!!Arc01:" << std::endl;		
				Cycle arc_cycle;
				if(shape_xy->getStartPoint()!=shape_xy->getEndPoint())
				{
					// no circle so find connected path
					g.dijkstra(*g.findVertex(shape_xy->getStartPoint()), *g.findVertex(shape_xy->getEndPoint()), arc_cycle);
				}
				bool add_arc_shape = arc_cycle.addShapeIfNotIn(*shape_xy);
				if(add_arc_shape==false)
				{
					std::cout << "!!!Shape not added:" << std::endl;
				}
				else
				{
					bool add_arc_graph = g_select_cycles.addGraphIfCycleNotIn(arc_cycle);
					
					if(add_arc_graph==false)
					{
						std::cout << "!!!Graph not added:" << std::endl;
					}
				}
			
			}	
		}
	}
	// sort cycle list by length
	g_select_cycles.sortByCycleLenght();
}

void Layer::calcEssentialCycles(const GraphList& gl_mcb, /*out*/GraphList& gl_J)
{
	
	gl_J.clear();
	GraphList gl_R(gl_mcb);
	GraphList gl_M;
	GraphList gl_Mequal;
	GraphList gl_Requal;
	unsigned int k=3;
	
	//cItGraphList it_cycle=g_select_cycles->graphs.begin(); 
	if(gl_R.graphs.size()<=1)
	{
		gl_J=gl_R;
	}
	else
	{
		while(1)
		{
			// get cycle with minimum |C|;
			//std::cout << "gl_R.graphs.size(): " << gl_R.graphs.size() << std::endl;
			gl_R.sortByCycleLenght();
			assert(gl_R.graphs.size()>0);
			Graph* C = gl_R.graphs.front()->clone();
			// R = R \ {C}
			////assert(gl_R->graphs.size()>1);
			gl_R.remove(*C);
			// if |C| > k or R = 0
			if(C->vertices.size() > k || gl_R.graphs.size() == 0)
			{
				// r = |M|
				unsigned int r = gl_M.graphs.size();
				////unsigned int r = gl_M.rank();
				// for all Graphs C' in M_equal
				for(cItGraphList it_c_=gl_Mequal.graphs.begin(); it_c_!=gl_Mequal.graphs.end(); ++it_c_)
				{
					assert(*it_c_);
					Graph* C_=(*it_c_)->clone();
					// if rank[(M + R_equal)\{C'}] < r then 
					GraphList gl_set(gl_M);
					gl_set.unionSet(gl_Requal);
					gl_set.remove(*C_);
					if(gl_set.rank() < r)
					{
						gl_J.addGraphIfNotIn(*C_);
					}
					if(C_ !=NULL)
					{
						delete C_;
						C_=NULL;
					}
				}
				// k = |C|; R_equal<-0; M_equal<-0
				k=C->vertices.size();
				gl_Requal.clear();
				gl_Mequal.clear();
			}
			// if R=0 then
			if(gl_R.graphs.size() == 0)
			{
				// return J (gl_J)
				break;
			}
			// R_equal <- R_equal unionSet {C}
			assert(C);
			gl_Requal.addGraphIfNotIn(*C);
			/* Extract of MCB */
			// if M unionSet {C} indent then
			GraphList gl_M_C(gl_M);
	/*
			std::cout << "Before Rank: a:" << gl_M_C.print() << std::endl;
			std::cout << "Before Rank Cycle C:" << C->print() << std::endl;
	*/
			gl_M_C.addGraphIfNotIn(*C);
	/*
			std::cout << "gl_M_C.rank:" << gl_M_C.rank() << std::endl;
			std::cout << "gl_M.rank():" << gl_M.rank() << std::endl;
			std::cout << "Rank: a:" << gl_M_C.print() << std::endl;
			std::cout << "Rank: b:" << gl_M.print() << std::endl;
	*/
			if( gl_M_C.rank() > gl_M.rank())
			{
				// M <- M unionSet {C}; M_equal <- M_equal unionSet {C}
				gl_M.addGraphIfNotIn(*C);
				gl_Mequal.addGraphIfNotIn(*C);
			}
			if(C!=NULL)
			{
				delete C;
				C=NULL;
			}
		}/*end_while(1)*/
	}
}

void Layer::calcGraphNoCycles(Graph& g_all, GraphList& g_all_cycles, GraphList& gl_no_cycle)
{
	gl_no_cycle.clear();
	// first get all shapes of the cycles
	Graph g_cycles_union;
	for(cItGraphList it=g_all_cycles.graphs.begin(); it!=g_all_cycles.graphs.end();++it)
	{
		Graph* g_temp=*it;
		assert(g_temp);
		g_cycles_union.unionSet(*g_temp);
	}
	
	// create difference of all graphs and all cycles
	Graph g_graph(g_all);
	std::cout << "All Graphs:" << std::endl;
	std::cout << g_graph.print() << std::endl;
	std::cout << "Cycle Union SET:" << std::endl;
	std::cout << g_cycles_union.print() << std::endl;
	g_graph.uniDirectDifferencSet(g_cycles_union);
	g_graph.intersectionSet(g_all);
	g_graph.createAllVertices();
	// return g_graph
	////
	////
	////
	////
	GraphList g_select_graphs;
	for(cItVertexList it_vs=g_graph.vertices.begin(); it_vs!=g_graph.vertices.end(); ++it_vs)
	{
		Vertex* vs=*it_vs;
		assert(vs);
		for(cItVertexList it_ve=g_graph.vertices.begin(); it_ve!=g_graph.vertices.end(); ++it_ve)
		{
			Vertex* ve=*it_ve;
			assert(ve);
			if(vs!=ve)
			{
				Graph path_list_vs_ve;
				g_graph.dijkstraMaxDist(*vs, *ve, path_list_vs_ve);
				if(path_list_vs_ve.shapes.size() > 0 )
				{
					////std::cout << "add to selcted graphs with no cycle:" << std::endl;
					////std::cout << path_list_vs_ve.print() << std::endl;
					g_select_graphs.addGraphIfNotIn(path_list_vs_ve);
				}
			}
		}
	}
	g_select_graphs.sortByCycleLenght();
	std::cout << "selcted graphs with no cycle:" << std::endl;
	std::cout << g_select_graphs.print() << std::endl;
	for(cRitGraphList rit_g=g_select_graphs.graphs.rbegin(); rit_g!=g_select_graphs.graphs.rend(); ++rit_g)
	{
		Graph* g=*rit_g;
		assert(g);
		bool is_indent=true;
		for(cItGraphList it_noc=gl_no_cycle.graphs.begin(); it_noc!=gl_no_cycle.graphs.end(); ++it_noc)
		{
			Graph* g_temp=*it_noc;
			assert(g_temp);
			Graph g_intersect(*g);
			g_intersect.intersectionSet(*g_temp);
			if(g_intersect.shapes.size()>0 )
			{
				is_indent=false;
				break;
			}
		}
		if(is_indent)
			gl_no_cycle.addGraphIfNotIn(*g);
	}
}

void Layer::calcConnections(/*out*/GraphList& gl_connections)
{
	Graph g_connect;
	//first put all Start and en point into the a new graph
	for(cItGraphList it_gl=m_all_cycles.graphs.begin(); it_gl!=m_all_cycles.graphs.end(); ++it_gl)
	{
		Graph* g_temp=*it_gl;
		assert(g_temp);
		g_temp->createAllVertices();
		if(g_temp->shapes.size()>0)
		{
			Vertex v_start;
			v_start.point=g_temp->getStartPoint();
			g_connect.addVertexIfNotIn(v_start);
		}
	}
	for(cItGraphList it_gl=m_all_not_cycles.graphs.begin(); it_gl!=m_all_not_cycles.graphs.end(); ++it_gl)
	{
		Graph* g_temp=*it_gl;
		assert(g_temp);
		if(g_temp->shapes.size()>0)
		{
			Vertex v_start;
			v_start.point=g_temp->getStartPoint();
			g_connect.addVertexIfNotIn(v_start);
			Vertex v_end;
			v_end.point=g_temp->getEndPoint();
			g_connect.addVertexIfNotIn(v_end);
		}
	}
	// calculate all posible connections 
	g_connect.createAllPossibleConnections();
	// sort shapes by length
	g_connect.sortShapesByLength();	
	// put each graph into a seperate graph list (tree)
	lGlList forest;
	for(cItGraphList it_gl=m_all_cycles.graphs.begin(); it_gl!=m_all_cycles.graphs.end(); ++it_gl)
	{
		Graph* g_temp=*it_gl;
		assert(g_temp);
		GraphList* gl_temp = new GraphList();
		gl_temp->addGraphIfNotIn(*g_temp);
		forest.push_back(gl_temp);
	}
	for(cItGraphList it_gl=m_all_not_cycles.graphs.begin(); it_gl!=m_all_not_cycles.graphs.end(); ++it_gl)
	{
		Graph* g_temp=*it_gl;
		assert(g_temp);
		GraphList* gl_temp = new GraphList();
		gl_temp->addGraphIfNotIn(*g_temp);
		forest.push_back(gl_temp);
	}
	//get the shortes connection
	while(g_connect.shapes.size() > 0)
	{
		Shape* s_connect = g_connect.shapes.front();
		if(s_connect)
		{
			g_connect.removeShape(*s_connect);
		}
		
		GraphList* gl_start=NULL;
		GraphList* gl_end=NULL;
		for(cItGlList it_f=forest.begin(); it_f!=forest.end(); ++it_f)
		{
			GraphList* gl_temp=*it_f;
			assert(gl_temp);
			Graph* g_front = gl_temp->graphs.front();
			Graph* g_back = gl_temp->graphs.back();
			if(g_back->getEndPoint() == s_connect->getStartPoint() )
			{
				//
				gl_start=gl_temp;
			}
			else if(g_front->getStartPoint() == s_connect->getEndPoint() )
			{
				gl_end=gl_temp;
			}
			if(gl_start != NULL && gl_end!=NULL)
			{
				// if the connection connect two graphs place the 2 graphs and the connection into 
				// one graphlist.
				Graph* g_connection = new ConnectionGraph();
				g_connection->addShapeIfNotIn(*s_connect);
				gl_start->addGraphIfNotIn(*g_connection);
				for(cItGraphList it_gl=gl_end->graphs.begin();it_gl!=gl_end->graphs.end(); ++ it_gl)
				{
					Graph* g_temp=*it_gl;
					assert(g_temp);
					gl_start->addGraphIfNotIn(*g_temp);
				}
				forest.remove(gl_end);
				delete gl_end;
				break;
			}
		}
	}
	assert(forest.size() <= 1);
	if(forest.size()==1)
	{
		gl_connections=*forest.front();
	}
	 	
}
