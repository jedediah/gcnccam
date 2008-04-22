/*
	File: DxfBlockList.cc
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
 
#include "DxfBlockList.h"
#include <assert.h>

DxfBlockList::DxfBlockList()
{
}
DxfBlockList::~DxfBlockList()
{
	clear();
}

DxfBlock* DxfBlockList::addBlock(const std::string& block_name, const Point2D& ref_point)
{
	DxfBlock* b_temp = find(block_name);
	if(b_temp==NULL)
	{
		b_temp = new DxfBlock(block_name, ref_point);
		m_blocks.push_back(b_temp);
	}
	return b_temp;
}

DxfBlock* DxfBlockList::find(const std::string name)
{
	DxfBlock* ret = NULL;
	for(cItBlockList it=m_blocks.begin(); it!=m_blocks.end(); ++it)
	{
		DxfBlock* b_temp=*it;
		assert(b_temp);
		if(name.compare(b_temp->getName())==0)
		{
			ret=b_temp;
			break;
		}
	}
	return ret;
}

const lBlockList& DxfBlockList::getBlockList()
{
	return m_blocks;
}


void DxfBlockList::clear()
{
	for(cItBlockList it=m_blocks.begin(); it!=m_blocks.end(); ++it)
	{
		DxfBlock* b_temp = *it;
		if(b_temp!=NULL)
		{
			delete b_temp;
			b_temp=NULL;
		}
	}
	m_blocks.clear();
}
