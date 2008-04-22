/*
	File: ConnectionGraph.cc
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

#include "ConnectionGraph.h"
#include "assert.h"

ConnectionGraph::ConnectionGraph() : Graph()
{
	// TODO: put constructor code here
}

ConnectionGraph::ConnectionGraph(const ConnectionGraph& cg) : Graph(cg)
{
}

ConnectionGraph::ConnectionGraph(const lShapeList& s) :Graph(s)
{
}

ConnectionGraph::~ConnectionGraph()
{
	// TODO: put destructor code here
}

Graph* ConnectionGraph::clone() const
{
	return new ConnectionGraph(*this);
}

std::ostream& ConnectionGraph::getCncCode(std::ostream &os, unsigned int& jump_mark)
{
	// z move is done here
	os << "G0" << " Z" << m_connection_property->getZStartPosition() << std::endl;
	for(cItShapeList it=shapes.begin(); it!=shapes.end(); ++it)
	{
		Shape* s_temp=*it;
		assert(s_temp);
		s_temp->getCncCode(os, jump_mark);
	}
	return os;

}
