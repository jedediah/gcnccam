/*
	File: GerberNcReader.h
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

#ifndef GERBERNCREADER_H
#define GERBERNCREADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <exception>
#include <stdexcept>
#include "Point2D.h"
#include "LayerList.h"
#include "Shape.h"
#include "Arc.h"

typedef std::map<std::string, double> TToolMap;
typedef std::pair<std::string, double> TToolPair;
typedef std::map<std::string, double> TToolMap;
typedef std::map<std::string, double>::iterator TToolMapIt;

class GerberNcReader
{
    public:
        GerberNcReader(LayerList& all_layers);
        virtual ~GerberNcReader();
        bool readFile(std::string file_name);
    protected:
    private:
	LayerList& m_layers;
    std::string readNextLine();
    std::string stripWhitespace(const std::string& in);
    Point2D getDrillPos(std::string line);
    double getFormatedValue(std::string string_val);
    bool readHeader();
    std::ifstream m_input_stream;
    bool m_is_metric;
    bool m_is_trailing_zero;
    bool m_separate_layers;
    TToolMap m_tool_map;
    unsigned int m_integer_places;
    unsigned int m_decimal_places;
    static const double INCH_TO_MM=25.4;
    static const unsigned int DEFAULT_INTEGER_PLACES=2;
    static const unsigned int DEFAULT_DECIMAL_PLACES=4;
};

#endif // GERBERNCREADER_H
