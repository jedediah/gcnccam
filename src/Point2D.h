/*
	File: Point2D.h
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

#ifndef _POINT2D_H_
#define _POINT2D_H_
#include <string>
#include <iostream>
#include <sstream>

const double cMinDeltaToBeEqal = 1E-10;

class Point2D
{
	public:
		Point2D();
		virtual ~Point2D();
		Point2D(const double xVal, const double yVal);
		Point2D(const Point2D &p);
		Point2D& operator=(const Point2D& p);
		std::string print(void) const;
		bool operator==(const Point2D& b)const;
		bool operator!=(const Point2D& b)const;
		Point2D operator+(const Point2D b) const;
		Point2D operator-(const Point2D b) const;
	public:
		double x;
		double y;
	
};


#endif	//_POINT2D_H_
