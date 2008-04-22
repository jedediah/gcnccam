/*
	File: DxfReader.h
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

#ifndef _DXFREADER_H_
#define _DXFREADER_H_
#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>
#include <vector>
#include <exception>
#include <stdexcept>
#include "Shape.h"
#include "Arc.h"
#include "Line.h"
//#include "PolygonBuilder.h"
#include "LayerList.h"
#include "DxfBlockList.h"
enum section { UNKNOWN, ENTITIES, BLOCKS };
class DxfReader
{
	public:
		DxfReader(LayerList& layers);
		virtual ~DxfReader();
		bool readFile(std::string file_name);
	
	private:
		bool check3d(void);
		void readNext2Lines(std::string& string1, std::string& string2);
		std::string readNextLine(void);
		std::string stripWhitespace(const std::string& in);
		DxfBlock* createBlock();
		void readLine(LayerListBase* layers);
		void readPolyline(LayerListBase* layers);
		void readCirle(LayerListBase* layers);
		void readArc(LayerListBase* layers);
		void readInsert();
	private:
		//std::istringstream input_stream;
		std::ifstream m_input_stream;
		LayerList& m_layers;
		section m_section;
		DxfBlockList m_blocks;
};


#endif	//_DXFREADER_H_
