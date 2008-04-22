/*
	File: DxfBlock.h
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

#ifndef _DXFBLOCK_H_
#define _DXFBLOCK_H_

#include "LayerListBase.h"
#include "Point2D.h"

class DxfBlock : public LayerListBase
{
	public:
		DxfBlock(const std::string& layer_name, const Point2D& ref_point);
		virtual ~DxfBlock();
		virtual void addShape(const std::string& layer_name, const Shape& shape);
		std::string getName();
		Point2D getRefPoint(); 
	protected:
		// DxfBlock variables
		std::string m_name;
		Point2D m_ref_point;
		// TODO: add member variables...
	
};


#endif	//_DXFBLOCK_H_
