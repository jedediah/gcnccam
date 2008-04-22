/*
	File: LayerPropertyList.cc
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

#include "LayerPropertyList.h"
#include "ConnectionProperty.h"
#include "assert.h"
#include <sstream>

LayerPropertyList::LayerPropertyList(ToolList* tl)
{
	assert(tl);
	m_tools=tl;
}


LayerPropertyList::~LayerPropertyList()
{
	clear();
}


LayerProperty* LayerPropertyList::find(const std::string name)
{
	LayerProperty* ret=NULL;
	if(name.compare("")!=0)
	{
		for(cItLayerPropertyList it=m_properties.begin(); it!=m_properties.end(); ++it)
		{
			LayerProperty* lp_temp=*it;
			assert(lp_temp);
			if(name.compare(lp_temp->getName())==0)
			{
				ret=lp_temp;
				break;
			}
		}		
	}
	return ret;
}

void LayerPropertyList::add(const LayerProperty& lp)
{

	if(lp.getName().compare("")!=0 && find(lp.getName())==NULL )
	{
		LayerProperty* lp_temp = lp.clone();
		m_properties.push_back(lp_temp);
		sortByPriority();
	}
}

void LayerPropertyList::remove(const std::string name)
{
	LayerProperty* lp_temp = find(name);
	if(lp_temp)
	{
		m_properties.remove(lp_temp);
		delete lp_temp;
	}
}

LayerProperty* LayerPropertyList::change(const LayerProperty& lp)
{
	LayerProperty* lp_temp=find(lp.getName());
	if(lp_temp)
	{
		*lp_temp=lp;
		sortByPriority();
	}
	return lp_temp;
}

const lLayerPropertyList& LayerPropertyList::getLayerPropertyList()
{
	return m_properties;
}

///
std::istream& operator>>(std::istream &is, LayerPropertyList& obj)
{
	std::string line;
	std::getline(is, line);
	while((line.compare("[LAYER_PROPERTY]")==0 || line.compare("[CONNECTION_PROPERTY]")==0) && is.good())
	{
		//found new layerproperty or connectionproperty
		
		if(line.compare("[LAYER_PROPERTY]")==0)
		{
			std::getline(is, line);
			std::stringstream st_lp;
			while(line.compare("[LAYER_PROPERTY]")!=0 && line.compare("[CONNECTION_PROPERTY]")!=0 && is.good())
			{
				st_lp << line << std::endl; //append line to new stream
				std::getline(is, line);	
			};
			LayerProperty lp = LayerProperty(obj.m_tools);	
			st_lp >> lp;
			obj.add(lp);
		}
		else if(line.compare("[CONNECTION_PROPERTY]")==0)
		{
			std::getline(is, line);
			std::stringstream st_cp;
			while(line.compare("[LAYER_PROPERTY]")!=0 && line.compare("[CONNECTION_PROPERTY]")!=0 && is.good())
			{
				st_cp << line << std::endl; //append line to new stream
				std::getline(is, line);	
			};
			ConnectionProperty cp;
			LayerProperty& pl=cp;
			st_cp >> pl;
			obj.add(pl);
		}
	};
	return is;
}

std::ostream& operator<<(std::ostream &os, const LayerPropertyList& obj)
{
	for(cItLayerPropertyList it=obj.m_properties.begin(); it!=obj.m_properties.end(); ++it)
	{
		LayerProperty* p_temp = *it;	
		assert(p_temp);
		if(p_temp!=NULL)
		{
			if(it==obj.m_properties.begin() && (typeid(*p_temp) != typeid(ConnectionProperty)))
			{
				std::cout << "LayerProperty::operator<< no ConnectionProperty found!" << std::endl;
				ConnectionProperty cp;
				LayerProperty& lp=cp;
				os << "[CONNECTION_PROPERTY]" << std::endl;
				os << lp;
			}
			if(typeid(*p_temp) == typeid(ConnectionProperty))
			{
				os << "[CONNECTION_PROPERTY]" << std::endl;
			}
			else
			{
				os << "[LAYER_PROPERTY]" << std::endl;
			}
			os << *p_temp;
		}
	}		
	return os;
}


void LayerPropertyList::clear()
{
	for(cItLayerPropertyList it=m_properties.begin(); it!=m_properties.end(); ++it)
	{
		LayerProperty* p_temp = *it;
		if(p_temp!=NULL)
		{
			delete p_temp;
			p_temp=NULL;
		}
	}
	m_properties.clear();
}
void LayerPropertyList::sortByPriority()
{
	m_properties.sort(comparePriority);
}

bool LayerPropertyList::comparePriority(LayerProperty* first, LayerProperty* second)
{
	bool ret = false;
	if ( first!=NULL && second!=NULL && first->getPriority() <  second->getPriority()) 
		ret=true;
	return ret;
}
