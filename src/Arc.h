/*
    File: Arc.h
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

#ifndef _ARC_H_
#define _ARC_H_
#include "Shape.h"

class Arc : public Shape
{
	public:
//		Arc();
//		Arc(const Point2D& s_point, const Point2D& e_point, const Point2D& center);
		Arc(const Point2D& center, double s_angle, double e_angle, double radius);
		Arc(const Arc& a);
		virtual ~Arc();
	
		Arc& operator=(const Arc& a);
		virtual Shape* clone(void) const;

		virtual bool operator==(const Shape& s)const;
		virtual bool operator!=(const Shape& s)const;
		virtual void addOffset(Point2D offset);
		virtual void swapDirection(void);
		virtual std::string print(void);	
		virtual Point2D getStartPoint(void);	
		virtual Point2D getEndPoint(void);	
		virtual double getMagnitude(void);
		virtual double getAngle(void);	
		virtual double getLength(void);
		virtual double getContourStartAngle(void);	
		virtual double getContourEndAngle(void);	
		virtual void getMaxClippingArea(Point2D& p_lowleft, Point2D& p_upright); 
		virtual void draw(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset);
		virtual void drawStartBar(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset);
		virtual void drawEndArrow(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset);
		virtual std::ostream& getCncCode(std::ostream &os, unsigned int& jump_mark);
		virtual LayerProperty* const getProperty()const;
		virtual void setProperty(LayerProperty* const lp);
		virtual LayerProperty* const getConnectionProperty()const;
		virtual void setConnectionProperty(LayerProperty* const cp);
		virtual bool isCircle();
		//
		double getStartAngle(void);
		double getEndAngle(void);
		double getRadius(void);
		Point2D getCenterPoint(void);
		bool isCounterClockWise(void);
		
	protected:
		
		Point2D center_point;
		double start_angle;
		double end_angle;
		double radius;
		bool dir_ccw;
	
};


#endif	//_ARC_H_
