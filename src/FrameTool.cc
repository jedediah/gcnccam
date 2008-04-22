/*
	File: FrameTool.cc
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

#include "config.h"
#include "FrameTool.hh"
#include <iostream>
#include <sstream>
#include <assert.h>
FrameTool::FrameTool(GlademmData *gmm_data, Tool& t, WindowMain* const main_win) : FrameTool_glade(gmm_data)
{
	m_main_window=main_win;
	assert(m_main_window);
	m_tool_before=t;
	fillData(m_tool_before);
}

void FrameTool::on_entry_tool_number_changed()
{
	///std::cout << "here we go!!!" << entry_tool_number->get_text() << std::endl;	
}

void FrameTool::on_button_ok_clicked()
{
	getData(m_tool_actual);
	if(m_main_window->on_tool_changed(m_tool_before, m_tool_actual))
	{
		m_tool_before=m_tool_actual;
		fillData(m_tool_actual);
	}
}
void FrameTool::on_button_cancel_clicked()
{
	fillData(m_tool_before);
}

void FrameTool::fillData(const Tool& t)
{
	// number
	std::stringstream number;
	number << t.getNumber();
	entry_tool_number->set_text(number.str());
	// len
	std::stringstream length;
	length << t.getLenght();
	entry_tool_len->set_text(length.str());
	// dia
	std::stringstream diam;
	diam << t.getDiameter();
	entry_tool_dia->set_text(diam.str());
	// comment
	entry_tool_comment->set_text(t.getComment());	
}

void FrameTool::getData(Tool& t)
{
	// number
	std::stringstream number;
	number << entry_tool_number->get_text();
	int n;
	number >> n;
	t.setNumber(n);
	// len
	std::stringstream length;
	length << entry_tool_len->get_text();
	double l;
	length >> l;
	t.setLength(l);
	// dia
	std::stringstream diam;
	diam << entry_tool_dia->get_text();
	double d;
	diam >> d;
	t.setDiameter(d);
	// comment
	t.setComment(entry_tool_comment->get_text());
}
