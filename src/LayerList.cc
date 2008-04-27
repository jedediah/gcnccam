/*
	File: LayerList.cc
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

#include "LayerList.h"
#include <assert.h>
#include <iostream>
#include <sstream>

LayerList::LayerList(LayerPropertyList* const layerprops, ConfigSettings* const config_settings) : LayerListBase()
{
	assert(layerprops);
	m_layerprops=layerprops;
	assert(config_settings);
	m_config_settings=config_settings;
}


LayerList::~LayerList()
{
	
}

void LayerList::calcGraphs(void)
{
	for(cItLayerList it=m_layers.begin(); it!=m_layers.end(); ++it)
	{
		Layer* l_temp = *it;
		assert(l_temp);
		if(l_temp!=NULL)
		{
			l_temp->calcGraphs();
		}
	}	
}

void LayerList::addShape(const std::string& layer_name, const Shape& shape)
{
	Layer* l_temp = find(layer_name);
	if(l_temp==NULL && layer_name.compare(ConnectionProperty::CON_PROPERTY_NAME())!=0)
	{
		l_temp = new Layer(layer_name);
		l_temp->setConfigSettings(m_config_settings);
		if(m_layerprops!=NULL)
		{
			l_temp->setProperty(m_layerprops->find(layer_name));
			l_temp->setConnectionProperty(m_layerprops->find(ConnectionProperty::CON_PROPERTY_NAME()));
		}
		m_layers.push_back(l_temp);
		sortByPriority();
	}
	assert(l_temp);
	l_temp->addShape(shape);
}

void LayerList::addList(LayerListBase& layer_list, Point2D insert_point)
{
	for(cItLayerList it=layer_list.getLayerList().begin(); it!=layer_list.getLayerList().end(); ++it)
	{
		Layer* l_temp = *it;
		if(l_temp!=NULL)
		{
			//get all shapes lShapeList
			for(cItShapeList it_s=l_temp->getInputGraph().shapes.begin(); it_s!=l_temp->getInputGraph().shapes.end(); ++it_s)
			{
				Shape* s_temp=*it_s;
				if(s_temp)
				{
					////ToDo 
					//
					/// here
					//
					Shape* s_clone=s_temp->clone();
					assert(s_clone);
					if(s_clone)
					{
						//add offset to Shape
						s_clone->addOffset(insert_point);
						addShape(l_temp->getName(), *s_clone);
						delete s_clone;
					}
				}
			}
		}
	}
}

void LayerList::print(void)
{
	////std::stringstream stream;
	////stream << std::endl << "Layers:" << std::endl;
	std::cout << "Layers:" << std::endl;
	for(cItLayerList it=m_layers.begin(); it!=m_layers.end(); ++it)
	{
		Layer* l_temp = *it;
		assert(l_temp);
		if(l_temp!=NULL)
		{
			std::cout << l_temp->print() << std::endl;
		}
	}
	////return stream.str();
}

void LayerList::reloadLayerProperties()
{
	for(cItLayerList it=m_layers.begin(); it!=m_layers.end(); ++it)
	{
		Layer* l_temp = *it;
		if(l_temp!=NULL)
		{
			l_temp->setProperty(m_layerprops->find(l_temp->getName()));
			l_temp->setConnectionProperty(m_layerprops->find(ConnectionProperty::CON_PROPERTY_NAME()));
			l_temp->calcGraphs();
		}
	}
	sortByPriority();
}

void LayerList::getMaxClippingArea(Point2D& p_lowleft, Point2D& p_upright)
{
	for(cItLayerList it=m_layers.begin(); it!=m_layers.end(); ++it)
	{
		Layer* l_temp = *it;
		if(l_temp!=NULL)
		{
			l_temp->getMaxClippingArea(p_lowleft, p_upright);
		}
	}
}

std::ostream& LayerList::getCncCode(std::ostream &os)
{
	m_jump_mark=JUMP_MARK_START_INDEX;
	//os.setf(std::ios::showbase);
	//os.setf(std::ios::showpoint);
	os.setf(std::ios::fixed, std::ios::floatfield);
	os.precision(3);
	//setf(ios_base::fixed, ios_base::floatfield);
	//init
	os << "(VARIABLES)" << std::endl;
	if(m_config_settings->getAddToolChangePos())
	{			
		os << "#" << NC_VAR_TOOL_CHANGE_X <<"=" << m_config_settings->getToolChangeXPos() 
			<< " (X Tool Change Position)" << std::endl;
		os << "#" << NC_VAR_TOOL_CHANGE_Y <<"=" << m_config_settings->getToolChangeYPos() 
			<< " (Y Tool Change Position)" << std::endl;
		os << "#" << NC_VAR_TOOL_CHANGE_Z <<"=" << m_config_settings->getToolChangeZPos() 
			<< " (Z Tool Change Position)" << std::endl;
	}
	if(m_config_settings->getUseToolSensor())
	{
		os << "#" << NC_VAR_TOOL_SENSOR_X <<"=" << m_config_settings->getToolSensorXPos() 
			<< " (X Tool Sensor Position)" << std::endl;
		os << "#" << NC_VAR_TOOL_SENSOR_Y <<"=" << m_config_settings->getToolSensorYPos() 
			<< " (Y Tool Sensor Position)" << std::endl;
		os << "#" << NC_VAR_TOOL_SENSOR_Z <<"=" << m_config_settings->getToolSensorZPos() 
			<< " (Z Tool Sensor Position)" << std::endl;
		os << "#" << NC_VAR_TOOL_SENSOR_HEIGHT <<"=" << m_config_settings->getToolSensorHeigth() 
			<< " (Z Tool Sensor Height)" << std::endl;
		os << "#" << NC_VAR_TOOL_SENSOR_TOUCH_OFF_HEIGHT <<"=" << m_config_settings->getToolSensorTouchOffZPos() 
			<< " (Z Tool Sensor Touch Off HEIGHT)" << std::endl;
		
	}
	if(m_config_settings->getAddExitPos())
	{			
		os << "#" << NC_VAR_EXIT_X <<"=" << m_config_settings->getExitXPos() 
			<< " (X Exit Position)" << std::endl;
		os << "#" << NC_VAR_EXIT_Y <<"=" << m_config_settings->getExitYPos() 
			<< " (Y Exit Position)" << std::endl;
		os << "#" << NC_VAR_EXIT_Z <<"=" << m_config_settings->getExitZPos() 
			<< " (Z Exit Position)" << std::endl;
	}
	os << "(END-VARIABLES)" << std::endl;
	os << "(INIT)" << std::endl;
	os << "G17" << " (XY-Plane)" << std::endl;
	os << "G40" << " (Radius Compensation off)" << std::endl;
	os << "G49" << " (No Tool Length Offset)" << std::endl;
	os << "G54" << " (Select Coordinate system 1)" << std::endl;
	os << "G80" << " (Cancel Modal Motion)" << std::endl;
	os << "G90" << " (Absolute Distance Mode)" << std::endl;
	if(m_config_settings->getUnit()==UNIT_MM)
	{
		os << "G21" << " (UNITS: MM)" << std::endl;
	}
	else
	{
		os << "G20" << " (UNITS: INCH)" << std::endl;
	}
	os << "G94" << " (UNITS per minute feed rate)" << std::endl;
	os << "(END-INIT)" << std::endl;
	Layer* l_before=NULL;
	bool first_layer=false;
	for(cItLayerList it=m_layers.begin(); it!=m_layers.end(); ++it)
	{
		
		
		Layer* l_temp = *it;
		if(l_temp!=NULL)
		{
			if(l_temp->getProperty()!=NULL && !first_layer)
			{
				first_layer=true;
				std::cout << "First Layer: " << l_temp->getName() << std::endl;
				l_temp->getCncCode(os, m_jump_mark,true, first_layer);
			}
			else
			{
				if(l_before && l_before->getProperty() && l_temp->getProperty() && l_before->getProperty()->getTool() 
					&& l_before->getProperty()->getToolNumber()==l_temp->getProperty()->getToolNumber())
				{
					l_temp->getCncCode(os, m_jump_mark,false, false); //supress tool change on cnc code
				}
				else
				{
					l_temp->getCncCode(os, m_jump_mark, true, false); // create cnc code with tool change
				}
			}
			l_before=l_temp;
		}
	}
	if(m_config_settings->getAddExitPos())
	{			
		// Exit Move
		os << "G0" << " G53" << " Z" << "#" << NC_VAR_EXIT_Z  
			<< " (Go absolut to Z Exit position)" << std::endl;
		os << "G0" << " G53" << " X" << "#" << NC_VAR_EXIT_X  
			<< " Y" << "#" << NC_VAR_EXIT_Y  
			<< " (Go absolut to Exit position)" << std::endl;
	}
	os << "M2" << std::endl;	// end of cnc program	
	return os;
}

void LayerList::sortByPriority()
{
	m_layers.sort(comparePriority);
}

bool LayerList::comparePriority(Layer* first, Layer* second)
{
	bool ret = false;
	if ( first!=NULL && second!=NULL)
	{
		LayerProperty* lp_first=first->getProperty();
		LayerProperty* lp_second=second->getProperty();
		if ( lp_first!=NULL && lp_second!=NULL)
		{
			if(lp_first->getPriority() <  lp_second->getPriority()) 
				ret=true;
		}
		if ( lp_first==NULL && second!=NULL)
			ret=true;
	}
	return ret;
}
