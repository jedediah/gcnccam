/*
	File: DxfBlockList.h
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

#ifndef _DXFBLOCKLIST_H_
#define _DXFBLOCKLIST_H_

#include "DxfBlock.h"

#include <list>
#include <string>
#include "Point2D.h"

typedef std::list<DxfBlock*> lBlockList;
typedef std::list<DxfBlock*>::iterator itBlockList;
typedef std::list<DxfBlock*>::const_iterator cItBlockList;

class DxfBlockList
{
	public:
	DxfBlockList();
	virtual ~DxfBlockList();
	virtual DxfBlock* addBlock(const std::string& bock_name, const Point2D& ref_point);
	virtual DxfBlock* find(const std::string name);
	const lBlockList& getBlockList();
	protected:
		//
		virtual void clear();
	protected:
		lBlockList m_blocks;
};

#endif	//_DXFBLOCKLIST_H_
