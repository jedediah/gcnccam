/*
	File: LayerListBase.h
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

#ifndef _LAYERLISTBASE_H_
#define _LAYERLISTBASE_H_

#include <list>
#include <string>
#include "Shape.h"
#include "Layer.h"
typedef std::list<Layer*> lLayerList;
typedef std::list<Layer*>::iterator itLayerList;
typedef std::list<Layer*>::const_iterator cItLayerList;

class LayerListBase
{
	public:
		LayerListBase();
		virtual ~LayerListBase();
		virtual void addShape(const std::string& layer_name, const Shape& shape)=0;
		virtual Layer* find(const std::string name);
		virtual lLayerList& getLayerList();
	protected:
		//
		virtual void clear();
		//
		lLayerList m_layers;
	
};


#endif	//_LAYERBASE_H_
