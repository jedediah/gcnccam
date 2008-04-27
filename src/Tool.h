/*
	File: Tool.h
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

#ifndef _TOOL_H_
#define _TOOL_H_
#include <string>
#include <iostream>
class Tool
{
	public:
		Tool();
		Tool(const Tool& t);
		 ~Tool();
		Tool& operator=(const Tool& t);	
		void setNumber(int n);
		int getNumber() const;
		void setDiameter(double d);
		double getDiameter() const;
		void setLength(double l);
		double getLenght() const;
		void setComment(std::string c);
		std::string getComment()const;
		void write(std::ostream &os);
		friend std::istream& operator>>(std::istream &is, Tool& obj);
		friend std::ostream& operator<<(std::ostream &os, const Tool& obj); 
	protected:
		int m_number;
		double m_length;
		double m_diameter;
		std::string m_comment;
};


#endif	//_TOOL_H_
