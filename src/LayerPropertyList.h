/*
	File: LayerPropertyList.h
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

#ifndef _LAYERPROPERTYLIST_H_
#define _LAYERPROPERTYLIST_H_
#include "LayerProperty.h"

#include <map>
#include <string>
#include <list>

//typedef std::map<std::string, LayerProperty*> mLayerPropertyMap;
//typedef std::map<std::string, LayerProperty*>::iterator itLayerPropertyMap;
//typedef std::map<std::string, LayerProperty*>::const_iterator cItLayerPropertyMap;
typedef std::list<LayerProperty*> lLayerPropertyList;
typedef std::list<LayerProperty*>::iterator itLayerPropertyList;
typedef std::list<LayerProperty*>::const_iterator cItLayerPropertyList;

class LayerPropertyList
{
	public:
		LayerPropertyList(ToolList* tl);
		 ~LayerPropertyList();
		
		LayerProperty* find(const std::string name);
		void add(const LayerProperty& lp);
		void remove(const std::string name);
		LayerProperty* change(const LayerProperty& lp);
		const lLayerPropertyList& getLayerPropertyList();
		///
		friend std::istream& operator>>(std::istream &is, LayerPropertyList& obj);
		friend std::ostream& operator<<(std::ostream &os, const LayerPropertyList& obj); 
		void clear();	
	protected:
		lLayerPropertyList m_properties;
		ToolList* m_tools;
		void sortByPriority();
		static bool comparePriority(LayerProperty* first, LayerProperty* second);

};


#endif	//_LAYERPROPERTYLIST_H_
