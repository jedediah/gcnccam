/*
	File: ConnectionGraph.h
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

#ifndef _CONNECTIONGRAPH_H_
#define _CONNECTIONGRAPH_H_
#include "Graph.h"

class ConnectionGraph : public Graph
{
	public:
		ConnectionGraph();
		ConnectionGraph(const ConnectionGraph& cg);
		ConnectionGraph(const lShapeList& s);
		virtual ~ConnectionGraph();
		virtual Graph* clone() const;
		
		virtual std::ostream& getCncCode(std::ostream &os, unsigned int& jump_mark);
	protected:
		// ConnectionGraph variables
	
		// TODO: add member variables...
	
};


#endif	//_CONNECTIONGRAPH_H_
