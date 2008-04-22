/*
	File: DxfBlock.cc
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

#include "DxfBlock.h"
#include <assert.h>

DxfBlock::DxfBlock(const std::string& block_name, const Point2D& ref_point) : LayerListBase()
{
	m_name=block_name;
	m_ref_point=ref_point;
}

DxfBlock::~DxfBlock()
{
	// TODO: put destructor code here
}

void DxfBlock::addShape(const std::string& layer_name, const Shape& shape)
{
	Layer* l_temp = find(layer_name);
	if(l_temp==NULL)
	{
		l_temp = new Layer(layer_name);
		m_layers.push_back(l_temp);
	}
	assert(l_temp);
	l_temp->addShape(shape);
}

std::string DxfBlock::getName()
{
	return m_name;
}

Point2D DxfBlock::getRefPoint()
{
	return m_ref_point;
}
