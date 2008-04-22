/*
	File: LayerList.h
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

#ifndef _LAYERLIST_H_
#define _LAYERLIST_H_
#include "LayerListBase.h"
#include "LayerPropertyList.h"
#include "ConfigSettings.h"



class LayerList : public LayerListBase
{
	public:
		LayerList(LayerPropertyList* const layerprops, ConfigSettings* const config_settings);
		virtual ~LayerList();
		virtual void addShape(const std::string& layer_name, const Shape& shape);
		void addList(LayerListBase& layer_list, Point2D insert_point);
		void calcGraphs(void);
		void print(void);
		
		void reloadLayerProperties();
		virtual void getMaxClippingArea(Point2D& p_lowleft, Point2D& p_upright);
		virtual std::ostream& getCncCode(std::ostream &os);
	protected:
		void sortByPriority();
		static bool comparePriority(Layer* first, Layer* second);
		ConfigSettings* m_config_settings;
		LayerPropertyList* m_layerprops;
		unsigned int m_jump_mark;
		static const unsigned int JUMP_MARK_START_INDEX=100;
};


#endif	//_LAYERLIST_H_
