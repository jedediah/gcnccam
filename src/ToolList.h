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

#ifndef _TOOLLIST_H_
#define _TOOLLIST_H_
#include "Tool.h"
#include <map>
typedef std::map<int, Tool*> mToolMap;
typedef std::map<int, Tool*>::iterator itToolMap;
typedef std::map<int, Tool*>::const_iterator cItToolMap;

class ToolList
{
	public:
		ToolList();
		 ~ToolList();
		Tool* find(const int number);
		void add(const Tool& t);
		void remove(const int& number);
		Tool* change(const Tool& t);
		friend std::istream& operator>>(std::istream &is, ToolList& obj);
		friend std::ostream& operator<<(std::ostream &os, const ToolList& obj); 
		const mToolMap& getToolMap();
		void clear();
		
	protected:
		mToolMap m_tools;
	
};


#endif	//_TOOLLIST_H_
