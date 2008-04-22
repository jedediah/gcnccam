/*
	File: GraphListVerticesBits.cc
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

#include "GraphListVerticesBits.h"


GraphListVerticesBits::GraphListVerticesBits(const lGraphList& graphs)
{
	// get list aff all Vertices by building the union set of all graphs
	Graph g_union;
	for(cItGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		g_union.unionSet(**it);
	}
	for(cItVertexList it_v=g_union.vertices.begin(); it_v!=g_union.vertices.end(); it_v++)
	{
		vertices.push_back(*it_v);
	}
	for(cItGraphList it=graphs.begin(); it!=graphs.end(); it++)
	{
		vBits vertex_bits;
		for(cItVertexList it_v=g_union.vertices.begin(); it_v!=g_union.vertices.end(); it_v++)
		{
			if((*it)->findVertex(**it_v))
				vertex_bits.push_back(true);
			else
				vertex_bits.push_back(false);
		}
		list_bits_vector.push_back(vertex_bits);
	}
}


GraphListVerticesBits::~GraphListVerticesBits()
{
	// TODO: put destructor code here
}

void GraphListVerticesBits::gauss(void)
{
/*
	list_bits_vector[1][0]=1;
	list_bits_vector[1][1]=1;
	list_bits_vector[1][2]=0;
	list_bits_vector[1][3]=0;
	list_bits_vector[1][4]=0;
	list_bits_vector[0][0]=0;
	list_bits_vector[0][1]=0;
	list_bits_vector[0][2]=1;
	list_bits_vector[0][3]=1;
	list_bits_vector[0][4]=1;
*/
	cItBits it_b;
	for(unsigned int k=0; k<vertices.size(); k++)
	{
		unsigned int s=k;	
		while(s<list_bits_vector.size() && list_bits_vector[s][k]==false)
		{
			s++;	
		}
		if(s<list_bits_vector.size())
		{
			//exchange bit vectors
			vBits temp_bits=list_bits_vector[s];
			list_bits_vector[s]=list_bits_vector[k];
			list_bits_vector[k]=temp_bits;
			for(unsigned int i=0; i<list_bits_vector.size(); i++)
			{
				if(i!=k && list_bits_vector[i][k]==true)
				{
					for(unsigned j=k; j<list_bits_vector[i].size(); j++)
					{
						list_bits_vector[i][j]=list_bits_vector[i][j]^list_bits_vector[k][j];
					}
				}
			}
		}
	}
	// erase emty bit vectors
	for(itBitsVector it_bv=list_bits_vector.begin(); it_bv!=list_bits_vector.end(); it_bv++)
	{
		vBits v_bits=*it_bv;
		bool val = false;
		for(cItBits it_b=v_bits.begin(); it_b!=v_bits.end();it_b++)
		{
			val |= *it_b;
		}
		if(val==false)
		{
			// all bits flase so erase the complete cycle
			list_bits_vector.erase(it_bv);
			--it_bv;
		}
	}
}

unsigned int GraphListVerticesBits::rank(void)
{
	gauss();
	return list_bits_vector.size();
}
std::string GraphListVerticesBits::print(void)
{
	std::stringstream stream;
	stream << "Vertices:";
	for(cItVertexList it_v=vertices.begin(); it_v!=vertices.end(); it_v++)
	{
		if(it_v!=vertices.begin())
				stream<< " ,";
		stream<< (*it_v)->point.print();
	}
	stream<<std::endl;
	
	for(cItBitsVector it_bv=list_bits_vector.begin(); it_bv!=list_bits_vector.end(); it_bv++)
	{
		vBits v_bits=*it_bv;
		stream << "Bits:";
		for(cItBits it_b=v_bits.begin(); it_b!=v_bits.end();it_b++)
		{
			if(it_b!=v_bits.begin())
				stream<< " ,";
			stream<< *it_b;
		}
		stream<<std::endl;
	}
	return stream.str(); 
}
