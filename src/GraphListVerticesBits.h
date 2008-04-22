/*
	File: GraphListVerticesBits.h
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

#ifndef _GRAPHLISTVERTICESBITS_H_
#define _GRAPHLISTVERTICESBITS_H_
#include "Graph.h"
#include <vector>
#include <list>
#include <string>
#include <sstream>


typedef std::list<Graph*> lGraphList;
typedef std::list<Graph*>::iterator ItGraphList;
typedef std::list<Graph*>::const_iterator cItGraphList;
typedef std::vector<bool> vBits;
typedef std::vector<bool>::iterator itBits;
typedef std::vector<bool>::const_iterator cItBits;
typedef std::vector<vBits> vBitsVector;
typedef std::vector<vBits>::iterator itBitsVector;
typedef std::vector<vBits>::const_iterator cItBitsVector;
typedef std::list<Vertex*> lVertexList;
typedef std::list<Vertex*>::const_iterator cItVertexList;
typedef std::list<Vertex*>::iterator itVertexList;

class GraphListVerticesBits
{
	public:
		GraphListVerticesBits(const lGraphList& graphs);
		 ~GraphListVerticesBits();
		
		void gauss(void);
		unsigned int rank(void);
		std::string print(void);
	private:
		vBitsVector list_bits_vector;
		lVertexList vertices;
};


#endif	//_GRAPHLISTVERTICESBITS_H_
