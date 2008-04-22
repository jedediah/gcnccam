/*
	File: Shape.h
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

#ifndef _SHAPE_H_
#define _SHAPE_H_
#include "Point2D.h"
#include "LayerProperty.h"
#include <string>
#include <iostream>
#include <sstream>
#include <gdkmm/window.h>
class Shape
{
	public:
		Shape();
		virtual ~Shape();
		virtual Shape* clone(void) const=0;
		virtual bool operator==(const Shape& s)const = 0;
		virtual bool operator!=(const Shape& s)const = 0;
		virtual void addOffset(Point2D offset)=0;
		virtual void swapDirection(void)=0;
		virtual std::string print(void)=0;	
		virtual Point2D getStartPoint(void)=0;	
		virtual Point2D getEndPoint(void)=0;	
		virtual double getMagnitude(void)=0;
		virtual double getAngle(void)=0;	
		virtual double getLength(void)= 0;
		virtual double getContourStartAngle(void)=0;	
		virtual double getContourEndAngle(void)=0;	
		virtual void getMaxClippingArea(Point2D& p_lowleft, Point2D& p_upright)=0; 
		virtual void draw(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)=0;
		virtual void drawStartBar(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)=0;
		virtual void drawEndArrow(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)=0;
		virtual std::ostream& getCncCode(std::ostream &os, unsigned int& jump_mark)=0;
		virtual std::ostream& getEndCncEntryMove(std::ostream &os, double z_zero_pos, double z_cut_pos);
		virtual Point2D getStartTangentPoint(double length, double offset, bool right_side=true);
		virtual Point2D getEndTangentPoint(double length, double offset, bool right_side=true);
		virtual LayerProperty* const getProperty()const=0;
		virtual void setProperty(LayerProperty* const lp)=0;
		virtual LayerProperty* const getConnectionProperty()const=0;
		virtual void setConnectionProperty(LayerProperty* const cp)=0;
		virtual bool isCircle();
		static const double ARROW_LENGHT=7;
		//static const double MIN_OFFSET_ENTRY_MOVE=0.001;
		static const double MIN_OFFSET_ENTRY_MOVE=0.002;
	protected:
		LayerProperty* m_property;
		LayerProperty* m_connection_property;		
};


#endif	//_SHAPE_H_
