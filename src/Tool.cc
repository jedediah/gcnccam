/*
	File: Tool.cc
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

#include "Tool.h"
#include <assert.h>
#include <sstream>

Tool::Tool()
{
	m_number=0;
	m_length=0.0;
	m_diameter=0.0;
	m_comment="New Tool";	
}

Tool::Tool(const Tool& t)
{
	m_number=t.m_number;
	m_length=t.m_length;
	m_diameter=t.m_diameter;
	m_comment=t.m_comment;	
}
Tool& Tool::operator=(const Tool& t)
{
	m_number=t.m_number;
	m_length=t.m_length;
	m_diameter=t.m_diameter;
	m_comment=t.m_comment;	
	return *this;	
}

Tool::~Tool()
{
	// TODO: put destructor code here
}

void Tool::setNumber(int n)
{
	m_number=n;
}

int Tool::getNumber() const
{
	return m_number;
}

void Tool::setDiameter(double d)
{
	m_diameter=d;
}

double Tool::getDiameter() const
{
	return m_diameter;
}

void Tool::setLength(double l)
{
	m_length=l;
}

double Tool::getLenght() const
{
	return m_length;
}

void Tool::setComment(std::string c)
{
	m_comment=c;
}

std::string Tool::getComment() const
{
	return m_comment;
}

std::istream& operator>>(std::istream &is, Tool& obj)
{
	do
	{
		std::string line;
		std::getline(is, line);
		std::string::size_type seperator=line.find_first_of("=");
		std::string variable=line.substr(0, seperator);
		std::string value=line.substr(seperator+1);
		int n;
		double d;
		//POC
		if(variable.compare("poc")==0)
		{
			std::stringstream number;
			number << value;
			number >> n;
			obj.setNumber(n);
		}
		//DIAMETER
		if(variable.compare("diameter")==0)
		{
			std::stringstream diameter;
			diameter << value;
			diameter >> d;
			obj.setDiameter(d);
		}
		//LENGTH
		if(variable.compare("length")==0)
		{
			std::stringstream length;
			length << value;
			length >> d;
			obj.setLength(d);
		}
		//COMMENT
		if(variable.compare("comment")==0)
		{
			obj.setComment(value);
		}
	}
	while(is.good());
	return is;
}

std::ostream& operator<<(std::ostream &os, const Tool& obj)
{
	//POC
	os << "poc" << "=" << obj.getNumber() << std::endl;
	//FMS
	os << "fms" << "=" << obj.getNumber() << std::endl;
	//LENGTH
	os << "length" << "=" << obj.getLenght() << std::endl;
	//DIAMETER
	os << "diameter" << "=" << obj.getDiameter() << std::endl;
	//COMMENT
	os << "comment" << "=" << obj.getComment() << std::endl;
	return os;
}
