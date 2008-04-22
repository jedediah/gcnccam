/*
	File: Vertex.h
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

#ifndef _VERTEX_H_
#define _VERTEX_H_
#include "Point2D.h"
#include "Shape.h"
class Vertex
{
	public:
		Vertex();
		virtual ~Vertex();
		Vertex(const Vertex& v);
		Vertex& operator=(const Vertex& v);
		bool operator==(const Vertex& v)const;
		bool operator!=(const Vertex& v)const;
		Point2D point;
		bool visited; //flag if vertex was visited by algorithm
		float dist; // distance to an other vertex
		Vertex* previous; // previus vertex
		Shape* connection; // connection to previous vertex
		
		// TODO: add member function declarations...
	
	protected:
		// Vertex variables
	
		// TODO: add member variables...
	
};


#endif	//_VERTEX_H_
