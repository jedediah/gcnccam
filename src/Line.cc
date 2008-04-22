/*
	File: Line.cc
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

#include "Line.h"
#include <math.h>
#include <assert.h>
Line::Line()
{
	m_property=NULL;
}

Line::Line(const Point2D& s_point, const Point2D& e_point)
{	
	start_point=s_point;
	end_point=e_point;
	m_property=NULL;
}

Line::Line(const Line& l)
{
	start_point=l.start_point;
	end_point=l.end_point;
	m_property=l.m_property;
}

Line::~Line()
{
	// TODO: put destructor code here
}

Line& Line::operator=(const Line& l)
{
	start_point=l.start_point;
	end_point=l.end_point;
	m_property=l.m_property;
	return *this;
}

Shape* Line::clone(void) const
{
	return new Line(*this);
}

bool Line::operator==(const Shape& s)const
{
	bool ret = false;
	if (typeid(s) == typeid(*this))
	{
		Line line=*(Line*)&s;
		ret = start_point == line.getStartPoint() 
				&& end_point == line.getEndPoint();
	}
	return ret;
}

bool Line::operator!=(const Shape& s)const
{
	return ! operator==(s);
}

void Line::addOffset(Point2D offset)
{
	end_point=end_point+offset;
	start_point=start_point+offset;
}

void Line::swapDirection(void)
{
	Point2D temp_p=end_point;
	end_point=start_point;
	start_point=temp_p;
}

std::string Line::print(void)
{
	std::stringstream stream;
	stream << "Line: start:" << start_point.print();
	stream << " ,end:" << end_point.print();
	stream << " ,polar:" << "@(" << getMagnitude() << " < " << getAngle() << ")";;
	return stream.str(); 
}

Point2D Line::getStartPoint(void)
{
	return start_point;
}

Point2D Line::getEndPoint(void)
{
	return end_point;
}

double Line::getMagnitude(void)
{
	double d_x = end_point.x - start_point.x;
	double d_y = end_point.y - start_point.y;
	double magnitude = sqrt(d_x * d_x + d_y * d_y);
	return magnitude;
}

double Line::getAngle(void)
{
	double d_x = end_point.x - start_point.x;
	double d_y = end_point.y - start_point.y;
	double angle = atan2 (d_y, d_x) / M_PI * 180.0 ;
	return angle;
}


double Line::getLength(void)
{
	return getMagnitude();
}

double Line::getContourStartAngle(void)
{
	return getAngle();
}

double Line::getContourEndAngle(void)	
{
	return getAngle();
}

void Line::getMaxClippingArea(Point2D& p_lowleft, Point2D& p_upright)
{
	//min x
	p_lowleft.x = fmin(p_lowleft.x, fmin(start_point.x, end_point.x));
	//min y
	p_lowleft.y = fmin(p_lowleft.y, fmin(start_point.y, end_point.y));
	//max x
	p_upright.x = fmax(p_upright.x, fmax(start_point.x, end_point.x));
	//max y
	p_upright.y = fmax(p_upright.y, fmax(start_point.y, end_point.y));
}

void Line::draw(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)
{
	Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(win);
	if(m_property)
	{
		gc->set_rgb_fg_color(m_property->getColor());
	}
	Point2D s_scaled;
	Point2D e_scaled;
	s_scaled.x=offset.x+(start_point.x*scale);
	s_scaled.y=offset.y-(start_point.y*scale);
	e_scaled.x=offset.x+(end_point.x*scale);
	e_scaled.y=offset.y-(end_point.y*scale);
	win->draw_line(gc, (int)(s_scaled.x), (int)(s_scaled.y), (int)e_scaled.x, (int)(e_scaled.y));
}

void Line::drawStartBar(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)
{
	Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(win);
	if(m_property)
	{
		gc->set_rgb_fg_color(m_property->getColor());
	}
	Point2D e_scaled;
	e_scaled.x=offset.x+(start_point.x*scale);
	e_scaled.y=offset.y-(start_point.y*scale);
	Point2D wing1;
	Point2D wing2;
	double line_angle=getAngle();
	double angle_wing1=(line_angle +180 -90)/180*M_PI;
	double angle_wing2=(line_angle +180 +90)/180*M_PI;
	wing1.x=e_scaled.x+cos(angle_wing1)*Shape::ARROW_LENGHT;
	wing1.y=e_scaled.y-sin(angle_wing1)*Shape::ARROW_LENGHT;
	wing2.x=e_scaled.x+cos(angle_wing2)*Shape::ARROW_LENGHT;
	wing2.y=e_scaled.y-sin(angle_wing2)*Shape::ARROW_LENGHT;
	win->draw_line(gc, (int)(e_scaled.x), (int)(e_scaled.y), (int)(wing1.x), (int)(wing1.y));
	win->draw_line(gc, (int)(e_scaled.x), (int)(e_scaled.y), (int)(wing2.x), (int)(wing2.y));
}

void Line::drawEndArrow(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)
{
	Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(win);
	if(m_property)
	{
		gc->set_rgb_fg_color(m_property->getColor());
	}
	Point2D e_scaled;
	e_scaled.x=offset.x+(end_point.x*scale);
	e_scaled.y=offset.y-(end_point.y*scale);
	Point2D wing1;
	Point2D wing2;
	double line_angle=getAngle();
	double angle_wing1=(line_angle +180 -20)/180*M_PI;
	double angle_wing2=(line_angle +180 +20)/180*M_PI;
	wing1.x=e_scaled.x+cos(angle_wing1)*Shape::ARROW_LENGHT;
	wing1.y=e_scaled.y-sin(angle_wing1)*Shape::ARROW_LENGHT;
	wing2.x=e_scaled.x+cos(angle_wing2)*Shape::ARROW_LENGHT;
	wing2.y=e_scaled.y-sin(angle_wing2)*Shape::ARROW_LENGHT;
	win->draw_line(gc, (int)(e_scaled.x), (int)(e_scaled.y), (int)(wing1.x), (int)(wing1.y));
	win->draw_line(gc, (int)(e_scaled.x), (int)(e_scaled.y), (int)(wing2.x), (int)(wing2.y));
}

std::ostream& Line::getCncCode(std::ostream &os, unsigned int& jump_mark)
{
	os << "G1" << " X" << end_point.x << " Y" << end_point.y << std::endl;
	return os;
}

LayerProperty* const Line::getProperty()const
{
	return m_property;
}

void Line::setProperty(LayerProperty* const lp)
{
	m_property=lp;
}	

LayerProperty* const Line::getConnectionProperty()const
{
	return m_connection_property;
}

void Line::setConnectionProperty(LayerProperty* const lp)
{
	m_connection_property=lp;
}
