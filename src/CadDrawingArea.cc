/*
    File: CadDrawingArea.cc
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

#include "CadDrawingArea.h"
#include <iostream>
#include <math.h>
#include <assert.h>
CadDrawingArea::CadDrawingArea(const lLayerList& layer_list, const Point2D p_lowleft, const Point2D p_upright)
		: Gtk::DrawingArea(), m_p_lowleft(p_lowleft), m_p_upright(p_upright)
{
	m_layers=layer_list;	
}


CadDrawingArea::~CadDrawingArea()
{
	// TODO: put destructor code here
}

bool CadDrawingArea::on_expose_event(GdkEventExpose* event)
{
   // This is where we draw on the window
  Glib::RefPtr<Gdk::Window> window = get_window();
  if(window)
  {
	window->clear();
	int x ,y , width, height, depth;
	window->get_geometry(x,y,width, height, depth);
	//////
	double width_no_border=(double)(width-2*BORDER_PIX);
	double height_no_border=(double)(height-2*BORDER_PIX);
	Point2D delta=m_p_upright-m_p_lowleft;
	double scale;
	if(delta.x/delta.y > width_no_border/height_no_border)
	{
		// drawing is wider than screen: scale to width
		scale=width_no_border/delta.x;
	}
	else
	{
		// drawing is narrow than screen: scale to height
		scale=height_no_border/delta.y;
	}
	Point2D offset;
	offset.x=double(BORDER_PIX)-(m_p_lowleft.x*scale); //pixel
	offset.y=height+(m_p_lowleft.y*scale)-double(BORDER_PIX); // pixel
	for(cItLayerList it=m_layers.begin(); it!=m_layers.end(); ++it)
	{
		Layer* l_temp = *it;
		if(l_temp!=NULL)
		{
			l_temp->draw(window, scale, offset);
		}
	}
  }
  return true;
}
