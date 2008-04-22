/*
	File: Connection.cc
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

#include "Connection.h"
#include <cmath>

Connection::Connection() : Line()
{
	// TODO: put constructor code here
}

Connection::Connection(const Point2D& s_point, const Point2D& e_point) 
	: Line(s_point, e_point)
{
}
Connection::Connection(const Connection& c)
	: Line(c)
{
}
Connection::~Connection()
{
	// TODO: put destructor code here
}

Connection& Connection::operator=(const Connection& c)
{
	start_point=c.start_point;
	end_point=c.end_point;
	m_property=c.getProperty();
	return *this;
}

Shape* Connection::clone(void) const
{
	return new Connection(*this);
}

bool Connection::operator==(const Shape& s)const
{
	bool ret = false;
	if (typeid(s) == typeid(*this))
	{
		Connection line=*(Connection*)&s;
		ret = start_point == line.getStartPoint() 
				&& end_point == line.getEndPoint();
	}
	return ret;
}

bool Connection::operator!=(const Shape& s)const
{
	return ! operator==(s);	
}

void Connection::draw(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)
{
	Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(win);
	if(m_connection_property)
	{
		gc->set_rgb_fg_color(m_connection_property->getColor());
	}
	Point2D s_scaled;
	Point2D e_scaled;
	s_scaled.x=offset.x+(start_point.x*scale);
	s_scaled.y=offset.y-(start_point.y*scale);
	e_scaled.x=offset.x+(end_point.x*scale);
	e_scaled.y=offset.y-(end_point.y*scale);
	win->draw_line(gc, (int)(s_scaled.x), (int)(s_scaled.y), (int)e_scaled.x, (int)(e_scaled.y));
}

void Connection::drawStartBar(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)
{
	//dummy we do not draw start bar on connections
}

void Connection::drawEndArrow(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)
{
	Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(win);
	if(m_connection_property)
	{
		gc->set_rgb_fg_color(m_connection_property->getColor());
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

std::ostream& Connection::getCncCode(std::ostream &os, unsigned int& jump_mark)
{
	// rapid move
	os << "G0" << " X" << end_point.x << " Y" << end_point.y << std::endl;
	return os;
}
