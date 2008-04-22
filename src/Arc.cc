/*
    File: Arc.cc
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

#include "Arc.h"
#include "Line.h"

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <assert.h>
/*
Arc::Arc() : Shape()
{
}
*/
/*
Arc::Arc(const Point2D& s_point, const Point2D& e_point, const Point2D& center)
	: Shape(s_point, e_point), center_point(center)
{
}
*/

Arc::Arc(const Point2D& center, double s_angle, double e_angle, double radius)
	: center_point(center)
{
	start_angle=s_angle;
	end_angle=e_angle;
	this->radius=radius;
	dir_ccw=true;
	m_property=NULL;
}


Arc::Arc(const Arc& a)
	:center_point(a.center_point)
{
	start_angle=a.start_angle;
	end_angle=a.end_angle;
	radius=a.radius;
	dir_ccw=a.dir_ccw;
	m_property=a.m_property;
}

Arc::~Arc()
{
	// TODO: put destructor code
}

Arc& Arc::operator=(const Arc& a)
{
	center_point=a.center_point;
	start_angle=a.start_angle;
	end_angle=a.end_angle;
	radius=a.radius;
	dir_ccw=a.dir_ccw;
	m_property=a.m_property;
	return *this;
}

Shape* Arc::clone(void) const
{
	return new Arc(*this);
}

bool Arc::operator==(const Shape& s)const
{
	bool ret = false;
	if (typeid(s) == typeid(*this))
	{
		Arc arc = *(Arc*)&s;
		ret = center_point == arc.center_point 
				&& end_angle == arc.end_angle
				&& start_angle == arc.start_angle
				&& radius == arc.radius
				&& dir_ccw == arc.dir_ccw;
	}
	return ret;
}
bool Arc::operator!=(const Shape& s)const
{
	return ! operator==(s);
}

void Arc::addOffset(Point2D offset)
{
	center_point=center_point+offset;
}

void Arc::swapDirection(void)
{
	double temp_angle=start_angle;
	start_angle=end_angle;
	end_angle=temp_angle;
	if(dir_ccw==true)
		dir_ccw=false;
	else
		dir_ccw=true;
}

std::string Arc::print(void)
{
	std::stringstream stream;
	stream << "Arc:";
	stream << "start-point:" << getStartPoint().print();
	stream << " ,end-point:" << getEndPoint().print();
	stream << " ,polar:" << "@(" << getMagnitude() << " < " << getAngle() << ")";
	stream << " ,center:" << center_point.print();
	stream << " ,start_angle:" << start_angle;
	stream << " ,end_angle:" << end_angle;
	stream << " ,r:" << radius;
	return stream.str();
}

Point2D Arc::getStartPoint(void)
{
	Point2D start_point;
	start_point.x = center_point.x + cos(start_angle*double(M_PIl)/double(180.0))*radius;
	start_point.y = center_point.y + sin(start_angle*double(M_PIl)/double(180.0))*radius;
	return start_point;
}
Point2D Arc::getEndPoint(void)
{
	Point2D end_point;
	end_point.x = center_point.x + cos(end_angle*double(M_PIl)/double(180.0))*radius;
	end_point.y = center_point.y + sin(end_angle*double(M_PIl)/double(180.0))*radius;	
	return end_point;
}

double Arc::getMagnitude(void)
{
	Point2D end_point(getEndPoint());
	Point2D start_point(getEndPoint());
	double d_x = end_point.x - start_point.x;
	double d_y = end_point.y - start_point.y;
	double magnitude = sqrt(d_x * d_x + d_y * d_y);
	return magnitude;
}

double Arc::getAngle(void)
{
	Point2D end_point(getEndPoint());
	Point2D start_point(getEndPoint());
	double d_x = end_point.x - start_point.x;
	double d_y = end_point.y - start_point.y;
	double angle = atan2 (d_y, d_x) / M_PI * 180.0 ;
	return angle;
}

double Arc::getLength(void)
{
	double radius = getRadius();
	double angle = fabs(start_angle - end_angle);
	double length = radius* M_PI * angle/double(180.0);
	return length;
}

double Arc::getContourStartAngle()
{
	double angle;
	if(dir_ccw)
	{
		angle=getStartAngle()+90.0;
	}
	else
	{
		angle=getStartAngle()-90;
	}
	return angle;
}

double Arc::getContourEndAngle()
{
	double angle;
	if(dir_ccw)
	{
		angle=getEndAngle()+90.0;
	}
	else
	{
		angle=getEndAngle()-90;
	}
	return angle;
}


void Arc::getMaxClippingArea(Point2D& p_lowleft, Point2D& p_upright)
{
    // first the simple version that use center and radius
	//min x
	p_lowleft.x = fmin(p_lowleft.x, center_point.x-radius);
	//min y
	p_lowleft.y = fmin(p_lowleft.y, center_point.y-radius);
	//max x
	p_upright.x = fmax(p_upright.x, center_point.x+radius);
	//max y
	p_upright.y = fmax(p_upright.y, center_point.y+radius);
}

void Arc::draw(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)
{
	
	Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(win);
	if(m_property)
	{
		gc->set_rgb_fg_color(m_property->getColor());
	}
	Point2D c_scaled;
	int p_x0= (int)((getCenterPoint().x-getRadius())*scale+offset.x);
	int p_y0= (int)(offset.y-(getCenterPoint().y+getRadius())*scale);
	int p_w= (int)((2.0*getRadius())*scale);
	int p_h= (int)((2.0*getRadius())*scale);
	int a0;
	int a1;
	// end angle < start angle
	if(dir_ccw)
	{
		std::cout << "Dir CCW" << std::endl;
		if(m_property && m_property->getCutDirection()==CCW)
			std::cout << "Dir Property CCW" << std::endl;
		if(getEndAngle() > getStartAngle() )
		{
			a0 = (int)(getStartAngle()*64);
			a1 = (int)((getEndAngle()-getStartAngle())*64);
		}
		else
		{
			a0 = (int)((getStartAngle())*64);
			a1 = (int)(((getEndAngle()+360)-getStartAngle())*64);
		}
	}
	else
		{
		if(getEndAngle() <= getStartAngle() )
		{
			a0 = (int)(getStartAngle()*64);
			a1 = (int)((getEndAngle()-getStartAngle())*64);
		}
		else
		{
			a0 = (int)((360.0+getStartAngle())*64);
			a1 = (int)((getEndAngle()-(getStartAngle()+360.0))*64);
		}
	}

	win->draw_arc(gc, false, p_x0, p_y0, p_w, p_h, a0, a1);
}

void Arc::drawStartBar(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)
{
	Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(win);
	if(m_property)
	{
		gc->set_rgb_fg_color(m_property->getColor());
	}
	Point2D e_scaled;
	e_scaled.x=offset.x+(getStartPoint().x*scale);
	e_scaled.y=offset.y-(getStartPoint().y*scale);
	Point2D wing1;
	Point2D wing2;
	double offset_angle=270;
	if(!dir_ccw)
		offset_angle=90;
	double angle_wing1=(start_angle +offset_angle -90)/180*M_PI;
	double angle_wing2=(start_angle +offset_angle +90)/180*M_PI;
	wing1.x=e_scaled.x+cos(angle_wing1)*Shape::ARROW_LENGHT;
	wing1.y=e_scaled.y-sin(angle_wing1)*Shape::ARROW_LENGHT;
	wing2.x=e_scaled.x+cos(angle_wing2)*Shape::ARROW_LENGHT;
	wing2.y=e_scaled.y-sin(angle_wing2)*Shape::ARROW_LENGHT;
	win->draw_line(gc, (int)(e_scaled.x), (int)(e_scaled.y), (int)(wing1.x), (int)(wing1.y));
	win->draw_line(gc, (int)(e_scaled.x), (int)(e_scaled.y), (int)(wing2.x), (int)(wing2.y));	
}

void Arc::drawEndArrow(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset)
{
	Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(win);
	if(m_property)
	{
		gc->set_rgb_fg_color(m_property->getColor());
	}
	Point2D e_scaled;
	e_scaled.x=offset.x+(getEndPoint().x*scale);
	e_scaled.y=offset.y-(getEndPoint().y*scale);
	Point2D wing1;
	Point2D wing2;
	double offset_angle=270;
	if(!dir_ccw)
		offset_angle=90;
	double angle_wing1=(end_angle +offset_angle -20)/180*M_PI;
	double angle_wing2=(end_angle +offset_angle +20)/180*M_PI;
	wing1.x=e_scaled.x+cos(angle_wing1)*Shape::ARROW_LENGHT;
	wing1.y=e_scaled.y-sin(angle_wing1)*Shape::ARROW_LENGHT;
	wing2.x=e_scaled.x+cos(angle_wing2)*Shape::ARROW_LENGHT;
	wing2.y=e_scaled.y-sin(angle_wing2)*Shape::ARROW_LENGHT;
	win->draw_line(gc, (int)(e_scaled.x), (int)(e_scaled.y), (int)(wing1.x), (int)(wing1.y));
	win->draw_line(gc, (int)(e_scaled.x), (int)(e_scaled.y), (int)(wing2.x), (int)(wing2.y));

}

std::ostream& Arc::getCncCode(std::ostream &os, unsigned int& jump_mark)
{
	Point2D p_offset=getCenterPoint() - getStartPoint();
	if(isCounterClockWise())
	{
		os << "G3";
	}
	else
	{
		os << "G2";
	}
	os << " X" << getEndPoint().x << " Y" << getEndPoint().y;
		os << " I" << p_offset.x << " J" << p_offset.y << std::endl;
	return os;
}

LayerProperty* const Arc::getProperty()const
{
	return m_property;
}

void Arc::setProperty(LayerProperty* const lp)
{
	m_property=lp;
}	

LayerProperty* const Arc::getConnectionProperty()const
{
	return m_connection_property;
}

void Arc::setConnectionProperty(LayerProperty* const lp)
{
	m_connection_property=lp;
}	

bool Arc::isCircle()
{
	bool ret = false;
	double angle=fabs(getEndAngle() - getStartAngle());
	if(angle==360.0)
		ret=true;
	return ret;
}

double Arc::getStartAngle(void)
{
	return start_angle;	
}

double Arc::getEndAngle(void)
{
	return end_angle;	
}

double Arc::getRadius(void)
{
	return radius;
}

Point2D Arc::getCenterPoint(void)
{
	return center_point;
}

bool Arc::isCounterClockWise(void)
{
	return dir_ccw;
}
