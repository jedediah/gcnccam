/*
	File: Point2D.cc
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

#include "Point2D.h"
#include <math.h>

Point2D::Point2D()
{
	x=0.0;
	y=0.0;
}
Point2D::Point2D(const double xVal, const double yVal)
{
	x=xVal;
	y=yVal;
}
Point2D::Point2D(const Point2D& p)
{
	x=p.x;
	y=p.y;
}
Point2D& Point2D::operator=(const Point2D& p)
{
	x=p.x;
	y=p.y;
	return *this;
}

std::string Point2D::print(void) const
{
	std::stringstream stream;
	stream << "(" << x << ", " << y << ")";
	return stream.str(); 
}

Point2D::~Point2D()
{
	// TODO: put destructor code here
}

bool Point2D::operator!=(const Point2D& b)const
{
	return ! operator==(b);
}
bool Point2D::operator==(const Point2D& b)const
{
	//return x==b.x && y==b.y;
	double dx = fabs(x-b.x);
	double dy = fabs(y-b.y);
	bool ret = false;
	if( dx < cMinDeltaToBeEqal && dy < cMinDeltaToBeEqal)
	{
		ret = true;
	}
	return ret;
}

Point2D Point2D::operator+(const Point2D b) const
{
	Point2D ret;
	ret.x=x+b.x;
	ret.y=y+b.y;
	return ret;
}

Point2D Point2D::operator-(const Point2D b) const
{
	Point2D ret;
	ret.x=x-b.x;
	ret.y=y-b.y;
	return ret;
}
