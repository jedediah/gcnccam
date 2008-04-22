/*
	File: GerberNcReader.cc
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

#include "GerberNcReader.h"
#include <math.h>

GerberNcReader::GerberNcReader(LayerList& all_layers) : m_layers(all_layers)
{
    m_is_metric=false;
    m_is_trailing_zero=false;
    m_separate_layers=false;
    m_integer_places=DEFAULT_INTEGER_PLACES;
    m_decimal_places=DEFAULT_DECIMAL_PLACES;
}

GerberNcReader::~GerberNcReader()
{
    //dtor
}


bool GerberNcReader::readFile(std::string file_name)
{
	bool is_nc_file=false;
	// clear shapes first
	//dxffile.exceptions(std::ios::failbit | std::ios::eof);
	m_input_stream.open(file_name.c_str());
	if(!m_input_stream)
		throw(std::runtime_error("File not found!"));

    std::string layer_name="drill";
	std::string line = "0";
	TToolMapIt it_tool=m_tool_map.end();
	std::cout << "Here we go GerberNcReader::readFile" << std::endl;
	do
	{
		std::cout << "line:" << line << std::endl;
		if(line.compare("M48")==0)
		{
			std::cout << "HEADER Found" << std::endl;
			is_nc_file=readHeader();
		}
		if(is_nc_file && line.compare(0,1,"M30")!=0)
		{
            if(line.compare(0,1,"T")==0)
            {
                std::cout << "USED Tool Found" << std::endl;
                it_tool=m_tool_map.find(line);
                if(it_tool!=m_tool_map.end()&& m_separate_layers)
                {
                    std::stringstream ss_layer_name;
                    ss_layer_name << "drill" << "_" << it_tool->second;
                    ss_layer_name >> layer_name;
                }
            }
            else if(it_tool!=m_tool_map.end()&&(line.compare(0,1,"X")==0 || line.compare(0,1,"x")==0))
            {
                // read values
                Point2D p_drill=getDrillPos(line);
                std::cout << "DRILL POS POINT:" << p_drill.print() << std::endl;
                //add arc to layer with radius (it_tool->second)/2.0
				double radius=(it_tool->second)/2.0;
				m_layers.addShape(layer_name, Arc(p_drill, 0.0, 360.0, radius));

            }
        }
		line=readNextLine();
	}while(!m_input_stream.eof()&&line.compare(0,1,"M30")!=0);
	//close file
	m_input_stream.close();
	return is_nc_file;
}

std::string GerberNcReader::readNextLine()
{
	std::string line_in;
	std::getline(m_input_stream, line_in);
	std::string::size_type return_pos=line_in.find_first_of('\r');
	std::string line_no_return=line_in.substr(0, return_pos);
    std::string line_nowhite=stripWhitespace(line_no_return);
	return line_nowhite;
}

std::string GerberNcReader::stripWhitespace(const std::string& in)
{
	std::string out(in);
	//trim	leading whitespace
	std::string::size_type notwhite=out.find_first_not_of(' ');
	out.erase(0, notwhite);
	notwhite=out.find_last_not_of(' ');
	out.erase(notwhite+1);
	return out;
}

Point2D GerberNcReader::getDrillPos(std::string line)
{
    Point2D p_drill;
    if(line.compare(0,1,"X")==0 || line.compare(0,1,"x")==0)
    {
        std::string line_x, line_y;
        std::string::size_type y_pos=line.find_first_of('Y');
        if(y_pos>=line.npos)
            y_pos=line.find_first_of('y');
        if(y_pos<line.npos)
        {
            line_x=line.substr(1, y_pos-1);
            line_y=line.substr(y_pos+1, line.npos);
            //std::cout << "DRILL POS: x:" << line_x << " ,y:" << line_y << std::endl;
            p_drill.x=getFormatedValue(line_x);
            p_drill.y=getFormatedValue(line_y);
            if(!m_is_metric)
            {
                //convert allway too mm
                p_drill.x=p_drill.x*INCH_TO_MM;
                p_drill.y=p_drill.y*INCH_TO_MM;
            }
        }
    }
    return p_drill;
}

double GerberNcReader::getFormatedValue(std::string string_val)
{
    double val=0.0;
    std::string string_integer;
    std::string string_dezimal;
    if(m_is_trailing_zero)
    {
        //X005600Y003200  X0056Y0032
        // start parsing from begin
        string_integer=string_val.substr(0, m_integer_places);
        string_dezimal=string_val.substr(m_integer_places,string_val.npos);
    }
    else //leading zerros
    {
        //X005600Y003200  X5600Y2300
        // start parsing from end
        //std::cout << "string_val.length():" << string_val.length() << std::endl;
        string_integer=string_val.substr(0, string_val.length()-m_decimal_places);
        //std::cout << "interger val:" << string_integer << std::endl;
        string_dezimal=string_val.substr(string_val.length()-m_decimal_places,string_val.npos);
        //std::cout << "dezimal val:" << string_dezimal << std::endl;
    }
    std::stringstream ss_val;
    ss_val << string_integer << "." << string_dezimal;
    ss_val >> val;
    return val;
}

bool GerberNcReader::readHeader()
{
    bool ret=true;
    m_tool_map.clear();
    std::string line = "0";
    line=readNextLine();
    //find unit
    std::string::size_type comma_pos=line.find_first_of(',');
    std::string unit=stripWhitespace(line.substr(0, comma_pos));
    std::cout << "UNIT:" << unit << std::endl;
    if(line.compare("METRIC")==0)
    {
        m_is_metric=true;
    }
    if(comma_pos<line.npos)
    {
        //find zero_cut
        std::string rest1=line.substr(comma_pos+1, line.npos);
        comma_pos=rest1.find_first_of(',');
        std::string zero_cut=stripWhitespace(rest1.substr(0, comma_pos));
        std::cout << "ZERO_CUT:" << zero_cut << std::endl;
        if(zero_cut.compare("TZ")==0)
        {
            m_is_trailing_zero=true;
        }
        if(comma_pos<rest1.npos)
        {
            //find format
            std::string rest2=rest1.substr(comma_pos+1, rest1.npos);
            comma_pos=rest2.find_first_of(',');
            std::string format=stripWhitespace(rest2.substr(0, comma_pos));
            std::cout << "FORMAT:" << format << std::endl;
            std::string::size_type dot_pos=format.find_first_of('.');
            if(dot_pos< format.npos)
            {

                m_integer_places=dot_pos;
                m_decimal_places=format.length()-1-dot_pos;
                std::cout << "FORMAT integer_places:" << m_integer_places << ", decimal_places:" << m_decimal_places <<std::endl;
            }
        }
    }
    do
	{
		line=readNextLine();
		std::string::size_type diameter_pos=line.find_first_of('C');
        std::string tool=line.substr(0, diameter_pos);
        if(diameter_pos<line.npos)
        {
            std::string diameter=line.substr(diameter_pos+1, line.npos);
            std::stringstream ss_diameter;
            ss_diameter << diameter;
            double d_diameter;
            ss_diameter >> d_diameter;
            if(!m_is_metric)
            {
                //convert allway too mm
                d_diameter=d_diameter*INCH_TO_MM;
            }
            std::cout << "ADD TOOL: " << tool << " , Diameter: " << d_diameter << std::endl;
            m_tool_map.insert(TToolPair(tool, d_diameter));
        }
	}while(line.compare("%") != 0 );
    return ret;
}
