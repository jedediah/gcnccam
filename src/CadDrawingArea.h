/*
    File: CadDrawingArea.h
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

#ifndef _CADDRAWINGAREA_H_
#define _CADDRAWINGAREA_H_
#include <gtkmm/drawingarea.h>
#include "LayerList.h"

class CadDrawingArea : public Gtk::DrawingArea
{
	public:
		CadDrawingArea(const lLayerList& layer_list, const Point2D p_lowleft, const Point2D p_upright);
		virtual ~CadDrawingArea();
protected:
  	//Override default signal handler:
  	virtual bool on_expose_event(GdkEventExpose* event);	
  
	lLayerList m_layers;
	const Point2D m_p_lowleft;
	const Point2D m_p_upright;
	static const int BORDER_PIX=10;
};


#endif	//_CADDRAWINGAREA_H_
