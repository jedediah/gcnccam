/*
	File: Vertex.cc
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

#include "Vertex.h"
#include <math.h>

Vertex::Vertex()
{
	previous = NULL;
	visited=false;
	dist=INFINITY;
	connection=NULL;
}
Vertex::Vertex(const Vertex& v)
{
	point=v.point;
	previous = v.previous;
	visited=v.visited;
	dist=v.dist;
	connection=v.connection;
}

Vertex& Vertex::operator=(const Vertex& v)
{
	point=v.point;
	previous = v.previous;
	visited=v.visited;
	dist=v.dist;
	connection=v.connection;
	return *this;
}
bool Vertex::operator==(const Vertex& v)const
{
	bool ret = point==v.point
			&& visited == v.visited
			&& dist == v.dist;
	if(ret==true)
	{
		if(previous==NULL && v.previous!=NULL)
			ret=false;
		if(previous!=NULL && v.previous==NULL)
			ret=false;
		if(previous!=NULL && v.previous!=NULL)
		{
			if(*previous!=*v.previous)
			{
				ret=false;
			}
		}
	}
	if(ret==true)
	{
		if(connection==NULL && v.connection!=NULL)
			ret=false;
		if(connection!=NULL && v.connection==NULL)
			ret=false;
		if(connection!=NULL && v.connection!=NULL)
		{
			if(*connection!=*v.connection)
			{
				ret=false;
			}
		}
	}
	return ret;
}

bool Vertex::operator!=(const Vertex& v)const
{
	return  !operator==(v);
}
Vertex::~Vertex()
{
	// TODO: put destructor code here
}
