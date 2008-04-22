/*
	File: DxfReader.cc
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

#include "DxfReader.h"
#include <assert.h>

DxfReader::DxfReader(LayerList& all_layers)
	: m_layers(all_layers)
{
	// TODO: put constructor code here
}

bool DxfReader::readFile(std::string file_name)
{
	m_section=UNKNOWN;
	bool ret=true;
	// clear shapes first
	//dxffile.exceptions(std::ios::failbit | std::ios::eof);
	m_input_stream.open(file_name.c_str());
	if(!m_input_stream)
		throw(std::runtime_error("File not found!"));
	//check 3d
	if(check3d())
	{
		std::cout << "!!!! 3D found " << std::endl;
		throw(std::runtime_error("Bad Structure on read dxffile: 3d not supported"));
	}
	
	std::cout << "!!!! 2D found " << std::endl;
	std::string line1 = "0";
	std::string line2 = "0";
	LayerListBase* layers=&m_layers;
	do
	{
		
		if(line1.compare("2")==0 && line2.compare("ENTITIES")==0)
		{
			m_section=ENTITIES;
			layers=&m_layers;
			std::cout << "ENTITIES Section!" << std::endl;
		}
		if(line1.compare("2")==0 && line2.compare("BLOCKS")==0)
		{
			m_section=BLOCKS;
			std::cout << "BLOCKS Section!" << std::endl;
		}
		if(line1.compare("0")==0 && line2.compare("ENDSEC")==0)
		{
			m_section=UNKNOWN;
			std::cout << "UNKNOWN Section!" << std::endl;
		}
		// echo 
		//std::cout << "line1:" << line1 << ", line2:" << line2 << std::endl;
		//
		if(m_section==BLOCKS)
		{
			if(line1.compare("0")==0 && line2.compare("BLOCK")==0)
			{
				std::cout << "block found " << std::endl;
				DxfBlock* b_temp=createBlock();
				assert(b_temp);
				if(b_temp)
					layers=b_temp;
			}
		}
		if(line1.compare("0")==0 && line2.compare("LINE")==0)
		{
			std::cout << "line found " << std::endl;
			readLine(layers);
		}			
		else if(line1.compare("0")==0 && line2.compare("LWPOLYLINE")==0)
		{
			std::cout << "lwpolyline found " << std::endl;
			readPolyline(layers);
		}
		else if(line1.compare("0")==0 && line2.compare("CIRCLE")==0)
		{
			std::cout << "circle found " << std::endl;
			readCirle(layers);
		}
		else if(line1.compare("0")==0 && line2.compare("ARC")==0)
		{
			std::cout << "arc found " << std::endl;
			readArc(layers);
		}
		else if(line1.compare("0")==0 && line2.compare("INSERT")==0)
		{
			std::cout << "INSERT found " << std::endl;
			readInsert();
		}
		readNext2Lines(line1, line2);
	}while(!m_input_stream.eof());
	//close file
	m_input_stream.close();
	return ret;
}

void DxfReader::readNext2Lines(std::string& string1, std::string& string2)
{
	string1 = readNextLine();
	string2 = readNextLine();
}

std::string DxfReader::readNextLine(void)
{
	std::string line;
	std::getline(m_input_stream, line);
	return stripWhitespace(line);
}

std::string DxfReader::stripWhitespace(const std::string& in)
{
	std::string out(in);
	//trim	leading whitespace
	std::string::size_type notwhite=out.find_first_not_of(' ');
	out.erase(0, notwhite);
	//std::string leading=in.substr(notwhite, in.npos);
	//trim trailing whitespace
	notwhite=out.find_last_not_of(' ');
	out.erase(notwhite+1);
	//std::string out=leading.substr(0, notwhite);
	//
	//std::cout << "in:" << in << ", out:" << out << std::endl;
	//
	return out;
}


bool DxfReader::check3d(void)
{
	bool ret=false;
	// store seek position 
	int pos=m_input_stream.tellg();
	std::string line="0";
	do
	{
		line=readNextLine();
		if(line.compare("70")==0) 
		{
			line=readNextLine();
			if(line.compare("192")==0)
			{
				ret=true;
				break;
			}
		}
	}while(!m_input_stream.eof());
	// restore seek position
	m_input_stream.clear();
	m_input_stream.seekg(pos);
	return ret;	
}

DxfBlock* DxfReader::createBlock()
{
	std::string line1;
	std::string line2;
	std::string block_name;
	Point2D ref_point;
	
	do
	{
		readNext2Lines(line1, line2);
		if(line1.compare("2")==0)
		{
			block_name=line2.data();
		}
		if(line1.compare("10")==0)
		{
			ref_point.x=atof(line2.data());
		}
		if(line1.compare("20")==0)
		{
			ref_point.y=atof(line2.data());
		}			
	}while(line1.compare("20") != 0 );
	return m_blocks.addBlock(block_name, ref_point);
}

void DxfReader::readLine(LayerListBase* layers)
{
	assert(layers);
	std::string line1;
	std::string line2;
	std::string layer_name;
	Point2D start_point;
	Point2D end_point;
	
	do
	{
		readNext2Lines(line1, line2);
		if(line1.compare("8")==0)
		{
			layer_name=line2.data();
		}
		if(line1.compare("10")==0)
		{
			start_point.x=atof(line2.data());
		}
		if(line1.compare("20")==0)
		{
			start_point.y=atof(line2.data());
		}		
		if(line1.compare("11")==0)
		{
			end_point.x=atof(line2.data());
		}		
		if(line1.compare("21")==0)
		{
			end_point.y=atof(line2.data());
		}		
	}while(line1.compare("21") != 0 );
	
	if(layers)
		layers->addShape(layer_name, Line(start_point, end_point));
}
void DxfReader::readPolyline(LayerListBase* layers)
{
	assert(layers);
	std::string line1;
	std::string line2;
	int counter = 0;
	int vertices_count = 1;
	int polyline_flag = 0;
	std::string layer_name;
	Point2D start_point;
	Point2D end_point;
	//
	do
	{
		readNext2Lines(line1, line2);
		if(line1.compare("8")==0)
		{
			layer_name=line2.data();
		}
		if(line1.compare("90")==0)
		{
			vertices_count=atoi(line2.data());
			//std::cout << "vertices_count:" << vertices_count << std::endl;
		}
		if(line1.compare("70")==0)
		{
			polyline_flag=atoi(line2.data());
		}	
		// new start point is old end point		
		if(line1.compare("10")==0)
		{
			end_point.x=atof(line2.data());
		}
		if(line1.compare("20")==0)
		{
			end_point.y=atof(line2.data());
			//std::cout << "vertice:" << end_point.print() << std::endl;
			counter++;	
			if(counter > 1 )
			{
				if(layers)
					layers->addShape(layer_name, Line(start_point, end_point));
			}
			start_point=end_point;
		}
	}while(counter < vertices_count );
}

void DxfReader::readCirle(LayerListBase* layers)
{
	assert(layers);
	std::string line1;
	std::string line2;
	std::string layer_name;
	Point2D center_point;
	double radius = 0;
	do
	{
		readNext2Lines(line1, line2);
		if(line1.compare("8")==0)
		{
			layer_name=line2.data();
		}
		if(line1.compare("10")==0)
		{
			center_point.x=atof(line2.data());
		}
		if(line1.compare("20")==0)
		{
			center_point.y=atof(line2.data());
		}		
		if(line1.compare("40")==0)
		{
			radius=atof(line2.data());
		}		
	}while(line1.compare("40") != 0 );
	if(layers)
		layers->addShape(layer_name, Arc(center_point, double(0), double(360), radius));
}

void DxfReader::readArc(LayerListBase* layers)
{
	assert(layers);
	std::string line1;
	std::string line2;
	std::string layer_name;
	Point2D center_point;
	double start_angle = 0;
	double end_angle = 0;
	double radius = 0;
	do
	{
		readNext2Lines(line1, line2);
		if(line1.compare("8")==0)
		{
			layer_name=line2.data();
		}
		if(line1.compare("10")==0)
		{
			center_point.x=atof(line2.data());
		}
		if(line1.compare("20")==0)
		{
			center_point.y=atof(line2.data());
		}		
		if(line1.compare("40")==0)
		{
			radius=atof(line2.data());
		}		
		if(line1.compare("50")==0)
		{
			start_angle=atof(line2.data());
		}	
		if(line1.compare("51")==0)
		{
			end_angle=atof(line2.data());
		}		
	}while(line1.compare("51") != 0 );
	if(layers)
		layers->addShape(layer_name, Arc(center_point, start_angle, end_angle, radius));
}

void DxfReader::readInsert()
{
	std::string line1;
	std::string line2;
	std::string block_name;
	Point2D insert_point;
	
	do
	{
		readNext2Lines(line1, line2);
		if(line1.compare("2")==0)
		{
			block_name=line2.data();
		}
		if(line1.compare("10")==0)
		{
			insert_point.x=atof(line2.data());
		}
		if(line1.compare("20")==0)
		{
			insert_point.y=atof(line2.data());
		}			
	}while(line1.compare("20") != 0 );
	
	DxfBlock* b_temp=m_blocks.find(block_name);
	assert(b_temp);
	if(b_temp)
	{
		m_layers.addList(*b_temp, insert_point);
	}
	
}


DxfReader::~DxfReader()
{

}
