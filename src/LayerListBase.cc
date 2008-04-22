/*
	File: LayerListBase.cc
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

#include "LayerListBase.h"
#include <assert.h>

LayerListBase::LayerListBase()
{
	
}


LayerListBase::~LayerListBase()
{
	clear();
}

Layer* LayerListBase::find(const std::string name)
{
	Layer* ret = NULL;
	for(cItLayerList it=m_layers.begin(); it!=m_layers.end(); ++it)
	{
		Layer* l_temp=*it;
		assert(l_temp);
		if(name.compare(l_temp->getName())==0)
		{
			ret=l_temp;
			break;
		}
	}
	return ret;
}

lLayerList& LayerListBase::getLayerList()
{
	return m_layers;
}

void LayerListBase::clear(void)
{
	for(cItLayerList it=m_layers.begin(); it!=m_layers.end(); ++it)
	{
		Layer* l_temp = *it;
		if(l_temp!=NULL)
		{
			delete l_temp;
			l_temp=NULL;
		}
	}
	m_layers.clear();
}
