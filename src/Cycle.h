/*
	File: Cycle.h
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

#ifndef _CYCLE_H_
#define _CYCLE_H_
#include "Graph.h"

class Cycle : public Graph
{
	public:
		Cycle();
		Cycle(const Graph& g);
		Cycle(const Cycle& g);
		virtual ~Cycle();
		virtual Graph* clone() const;
		virtual void sortCounterClockWise();
		virtual void sortClockWise();
		virtual std::ostream& getCncCode(std::ostream &os, unsigned int& jump_mark);
		virtual bool isCircle();
	protected:
		void swapDirection();
		bool dir_ccw;
		void sortVerticesLikeShapes();
		bool sortShapesAsRing(const Vertex& v_start, /*out*/ lShapeList& sl_ring);
		bool isCounterClockWise();
};


#endif	//_CYCLE_H_
