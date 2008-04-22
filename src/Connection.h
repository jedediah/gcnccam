/*
	File: Connection.h
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

#ifndef _CONNECTION_H_
#define _CONNECTION_H_
#include "Line.h"

class Connection : public Line
{
	public:
		Connection();
		Connection(const Point2D& s_point, const Point2D& e_point);
		Connection(const Connection& c);
		~Connection();
		Connection& operator=(const Connection& c);
		virtual Shape* clone(void) const;
		virtual bool operator==(const Shape& s)const;
		virtual bool operator!=(const Shape& s)const;
		virtual void draw(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset);
		virtual void drawStartBar(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset);
		virtual void drawEndArrow(Glib::RefPtr<Gdk::Window> win, const double scale, const Point2D offset);

		// Connection interface
	
		// TODO: add member function declarations...
		virtual std::ostream& getCncCode(std::ostream &os, unsigned int& jump_mark);
	protected:
		// Connection variables
	
		// TODO: add member variables...
	
};


#endif	//_CONNECTION_H_
