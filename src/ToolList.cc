/*
	File: ToolList.h
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

#include "ToolList.h"
#include <assert.h>
#include <sstream>

ToolList::ToolList()
{
	// TODO: put constructor code here
}


ToolList::~ToolList()
{
	clear();
}

Tool* ToolList::find(const int number)
{
	Tool* ret=NULL;
	cItToolMap it=m_tools.find(number);
	if(it!=m_tools.end())
	{
		ret = it->second;
	}
	return ret;
}

void ToolList::add(const Tool& t)
{
	cItToolMap it=m_tools.find(t.getNumber());
	if(it==m_tools.end())
	{
		Tool* t_temp = new Tool(t);
		std::pair<int, Tool*> p_temp(t.getNumber(), t_temp);
		m_tools.insert(p_temp);
	}
}

void ToolList::remove(const int& number)
{
	itToolMap it=m_tools.find(number);
	m_tools.erase(it);
}

Tool* ToolList::change(const Tool& t)
{
	Tool* ret=NULL;
	if(find(t.getNumber()))
	{
		cItToolMap it=m_tools.find(t.getNumber());
		Tool* t_temp=it->second;
		assert(t_temp);
		*t_temp=t;
		ret=t_temp;
	}
	return ret;
}
void ToolList::clear(void)
{
	for(cItToolMap it=m_tools.begin(); it!=m_tools.end(); ++it)
	{
		Tool* t_temp = it->second;
		if(t_temp!=NULL)
		{
			delete t_temp;
			t_temp=NULL;
		}
	}
	m_tools.clear();
}

const mToolMap& ToolList::getToolMap()
{
	return m_tools;
}
std::istream& operator>>(std::istream &is, ToolList& obj)
{
	std::string line;
	std::getline(is, line);
	while(line.compare("[TOOL]")==0 && is.good())
	{
		//found new lool
		std::getline(is, line);
		std::stringstream st_tool;
		while(line.compare("[TOOL]")!=0 && is.good())
		{
			st_tool<< line << std::endl; //append line to new stream
			std::getline(is, line);	
		};
		Tool t;
		st_tool >> t;
		obj.add(t);
	};
	return is;
}
std::ostream& operator<<(std::ostream &os, const ToolList& obj)
{
	for(cItToolMap it=obj.m_tools.begin(); it!=obj.m_tools.end(); ++it)
	{
		Tool* t_temp = it->second;
		assert(t_temp);
		if(t_temp!=NULL)
		{
			os << "[TOOL]" << std::endl;
			os << *t_temp;
		}
	}
	return os;
}
