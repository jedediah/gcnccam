/*
	File: FrameLayerProperty.cc
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
#include "FrameLayerProperty.hh"
#include <string>
#include <iostream>
#include <sstream>
#include <assert.h>
#include "ConnectionProperty.h"

FrameLayerProperty::FrameLayerProperty(GlademmData *gmm_data, LayerProperty& lp, ToolList* tools) 
	: FrameLayerProperty_glade(gmm_data)
{  
	m_is_connection=false;
	m_tools=tools;
	assert(m_tools);
	createColorButton();
	m_lp_before = new LayerProperty(lp);
	if(typeid(lp) == typeid(ConnectionProperty))
	{
		m_is_connection=true;
	}
	fillData(*m_lp_before);
}

FrameLayerProperty::~FrameLayerProperty()
{
	if(m_lp_before)
		delete m_lp_before;
}

void FrameLayerProperty::fillData(const LayerProperty& lp)
{
	//NAME
	entry_property_name->set_text(lp.getName());
	//COMMENT
	entry_comment->set_text(lp.getComment());
	//COLOR
	m_button_collor->set_color(lp.getColor());
	//PRIORITY
	std::stringstream priority;
	priority << lp.getPriority();
	entry_priority->set_text(priority.str());
	//CUT DIRECTION
	fillComboDirection(lp.getCutDirection());
	//CUT POSITON
	fillComboPosition(lp.getCutPosition());
	//CUT FFED RATE 
	std::stringstream cut_feed_rate;
	cut_feed_rate << lp.getCutFeedRate();
	entry_cut_feed_rate->set_text(cut_feed_rate.str());
	// Z Start Position
	std::stringstream z_start_positon;
	z_start_positon << lp.getZStartPosition();
	entry_z_start_position->set_text(z_start_positon.str());
	// Cut Depth
	std::stringstream z_cut_depth;
	z_cut_depth << lp.getCutDepth();
	entry_cut_depth->set_text(z_cut_depth.str());
	// Z Pitch/Cycle
	std::stringstream zpitch;
	zpitch << lp.getZPitch();
	entry_zpitch->set_text(zpitch.str());
	// Z FEED RATE
	std::stringstream z_feed_rate;
	z_feed_rate << lp.getZFeedRate();
	entry_z_feed_rate->set_text(z_feed_rate.str());
	//DRILL_SMALLER
	check_drill_smaller->set_active(lp.getForceDrillSmaller());
	//Tool
	fillComboTools(lp.getTool());
	// SPINDEL SPEED
	std::stringstream s_speed;
	s_speed << lp.getSpindleSpeed();
	entry_spindlespeed->set_text(s_speed.str());
	//COOLING_MIST
	check_cooling_mist->set_active(lp.getCoolingMist());
	//COOLING_FLOOD
	if(lp.getCoolingMist())
		check_cooling_flood->set_sensitive(true);
	else	
		check_cooling_flood->set_sensitive(false);
	check_cooling_flood->set_active(lp.getCoolingFlood());
	if(m_is_connection)
	{
		entry_property_name->set_sensitive(false);
		entry_comment->set_sensitive(false);
		entry_priority->set_sensitive(false);
		combo_direction->set_sensitive(false);
		combo_position->set_sensitive(false);
		entry_cut_feed_rate->set_sensitive(false);		
		entry_cut_depth->set_sensitive(false);
		entry_zpitch->set_sensitive(false);
		entry_z_feed_rate->set_sensitive(false);
		check_drill_smaller->set_sensitive(false);
		combo_tool->set_sensitive(false);
		entry_spindlespeed->set_sensitive(false);
		check_cooling_mist->set_sensitive(false);
		check_cooling_flood->set_sensitive(false);
	}
}

void FrameLayerProperty::getData(LayerProperty& lp)
{
	int n;
	double d;
	//NAME
	std::stringstream name;
	name << entry_property_name->get_text();
	lp.setName(name.str());
	//COMMENT
	lp.setComment(entry_comment->get_text());
	//COLOR
	lp.setColor(m_button_collor->get_color());
	//PRIORITY
	std::stringstream priority;
	priority << entry_priority->get_text();
	priority >> n;
	lp.setPriority(n);
	//CUT DIRECTION
	lp.setCutDirection((cutdirection)combo_direction->get_active_row_number());
	//CUT POSITON
	lp.setCutPosition((cutposition)combo_position->get_active_row_number());
	//CUT FEED RATE
	std::stringstream cut_feed_rate;
	cut_feed_rate << entry_cut_feed_rate->get_text();
	cut_feed_rate >> d;
	lp.setCutFeedRate(d);
	// Z Start Position
	std::stringstream z_start_positon;
	z_start_positon << entry_z_start_position->get_text();
	z_start_positon >> d;
	lp.setZStartPosition(d);
	// Cut Depth
	std::stringstream z_cut_depth;
	z_cut_depth << entry_cut_depth->get_text();
	z_cut_depth >> d;
	lp.setCutDepth(d);
	// Z Pitch/Cycle
	std::stringstream zpitch;
	zpitch << entry_zpitch->get_text();
	zpitch >> d;
	lp.setZPitch(d);
	//Z FEED RATE
	std::stringstream z_feed_rate;
	z_feed_rate << entry_z_feed_rate->get_text();
	z_feed_rate >> d;
	lp.setZFeedRate(d);
	//DRILL_SMALLER
	lp.setForceDrillSmaller(check_drill_smaller->get_active());
	//Tool
	Gtk::TreeModel::iterator it=combo_tool->get_active();
	if(it)
	{
		Gtk::TreeModel::Row row = *it;
		int id = row[m_ColumnsTool.m_col_id];
		lp.setToolNumber(id);
	}
	// SPINDEL SPEED
	std::stringstream s_speed;
	s_speed << entry_spindlespeed->get_text();
	s_speed >> d;
	lp.setSpindleSpeed(d);
	//COOLING MIST
	lp.setCoolingMist(check_cooling_mist->get_active());	
	//COOLING FLOOD
	if(lp.getCoolingMist())
		check_cooling_flood->set_sensitive(true);
	else	
		check_cooling_flood->set_sensitive(false);
	lp.setCoolingFlood(check_cooling_flood->get_active());
}

void FrameLayerProperty::on_button_ok_clicked()
{
	LayerProperty lp_actual(m_tools);
	getData(lp_actual);
	assert(m_lp_before);
	LayerProperty lp_old(*m_lp_before);
	if(m_signal_data_changed.emit(lp_old, lp_actual))
	{
		try
		{
			if(m_lp_before)
				delete m_lp_before;
			m_lp_before=new LayerProperty(lp_actual);
		}
		catch(...)
		{
			std::cout << "Error on FrameLayerProperty::on_button_ok_clicked()!!!" << std::endl;
		}
	}
}
void FrameLayerProperty::on_button_cancel_clicked()
{
}

void FrameLayerProperty::on_check_cooling_mist_toggled()
{
	//COOLING FLOOD
	if(check_cooling_mist->get_active())
		check_cooling_flood->set_sensitive(true);
	else	
	{
		check_cooling_flood->set_sensitive(false);
		check_cooling_flood->set_active(false);
	}
}

void FrameLayerProperty::createColorButton()
{
	m_button_collor = Gtk::manage(new class Gtk::ColorButton());
	m_button_collor->set_flags(Gtk::CAN_FOCUS);
	hbox_color->pack_start(*m_button_collor, Gtk::PACK_SHRINK, 0);
	m_button_collor->show();	
}
void FrameLayerProperty::fillComboDirection(const cutdirection dir)
{
	bool was_created_before=true;
	if(!m_refTreeModelDirection)
	{
		m_refTreeModelDirection = Gtk::ListStore::create(m_ColumnsDirection);
		was_created_before=false;
	}
	m_refTreeModelDirection->clear();
	combo_direction->set_model(m_refTreeModelDirection);
	Gtk::TreeModel::Row row = *(m_refTreeModelDirection->append());
	row[m_ColumnsDirection.m_col_id] = 1;
	row[m_ColumnsDirection.m_col_name] = "Counter Clockwise";
	row = *(m_refTreeModelDirection->append());
	row[m_ColumnsDirection.m_col_id] = 2;
	row[m_ColumnsDirection.m_col_name] = "Clockwise";
	if(!was_created_before)
		combo_direction->pack_start(m_ColumnsDirection.m_col_name);
	combo_direction->set_active(dir);
	combo_direction->show_all_children();	
}

void FrameLayerProperty::fillComboPosition(const cutposition pos)
{
	bool was_created_before=true;
	if(!m_refTreeModelPosition)
	{
		m_refTreeModelPosition = Gtk::ListStore::create(m_ColumnsPosition);
		was_created_before=false;
	}
	m_refTreeModelPosition->clear();
	combo_position->set_model(m_refTreeModelPosition);
	Gtk::TreeModel::Row row = *(m_refTreeModelPosition->append());
	row[m_ColumnsPosition.m_col_id] = 1;
	row[m_ColumnsPosition.m_col_name] = "Outside";
	row = *(m_refTreeModelPosition->append());
	row[m_ColumnsPosition.m_col_id] = 2;
	row[m_ColumnsPosition.m_col_name] = "Inside";
	row = *(m_refTreeModelPosition->append());
	row[m_ColumnsPosition.m_col_id] = 3;
	row[m_ColumnsPosition.m_col_name] = "Center";
	if(!was_created_before)
		combo_position->pack_start(m_ColumnsPosition.m_col_name);
	combo_position->set_active(pos);
	combo_position->show_all_children();	
}

void FrameLayerProperty::fillComboTools(Tool* t)
{
	bool was_created_before=true;
	if(!m_refTreeModelTool)
	{
		m_refTreeModelTool = Gtk::ListStore::create(m_ColumnsTool);
		was_created_before=false;
	}
	m_refTreeModelTool->clear();
	combo_tool->set_model(m_refTreeModelTool);
	Gtk::TreeModel::Row row;
	int selected=LayerProperty::NO_TOOL();
	int index=0;
	for(cItToolMap it=m_tools->getToolMap().begin(); it!=m_tools->getToolMap().end(); ++it)
	{
		Tool* t_temp = it->second;
		assert(t_temp);
		if(t_temp!=NULL)
		{
			row = *(m_refTreeModelTool->append());
			row[m_ColumnsTool.m_col_id] = t_temp->getNumber();
			row[m_ColumnsTool.m_col_name] = t_temp->getComment();
			if(t!=NULL && t_temp->getNumber()==t->getNumber())
				selected=index;
			index++;
		}
	}
	if(!was_created_before)
	{
		combo_tool->pack_start(m_ColumnsTool.m_col_id);
		combo_tool->pack_start(m_ColumnsTool.m_col_name);
	}
	if(selected!=LayerProperty::NO_TOOL())
		combo_tool->set_active(selected);
	combo_tool->show_all_children();	
}

FrameLayerProperty::type_signal_data_changed FrameLayerProperty::signal_data_changed()
{
	return m_signal_data_changed;
}
