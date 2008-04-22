/*
	File: DialogSetting.cc
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
#include "DialogSetting.hh"
#include <string>
#include <iostream>
#include <sstream>
#include <assert.h>

DialogSetting::DialogSetting(const ConfigSettings& conf) 
 : DialogSetting_glade()
{
	fillData(conf);
}



DialogSetting::~DialogSetting()
{
}

void DialogSetting::fillData(const ConfigSettings& conf)
{
	fillComboUnit(conf.getUnit());
	label_tool_change_pos_unit->set_text(conf.getLengthUnit());
	label_tool_sensor_pos_unit->set_text(conf.getLengthUnit());
	label_unit_length->set_text(conf.getLengthUnit());
	label_tool_sensor_height_unit->set_text(conf.getLengthUnit());
	label_touchoff_unit->set_text(conf.getLengthUnit());
	label_unit_speed->set_text(conf.getSpeedUnit());
	label_probe_feed_rate_unit->set_text(conf.getSpeedUnit());
	label_exit_pos_unit->set_text(conf.getLengthUnit());
	//add_tool_change_pos
	check_tool_change->set_active(conf.getAddToolChangePos());
	if(!conf.getAddToolChangePos())
	{
		entry_tool_change_x->set_sensitive(false);
		entry_tool_change_y->set_sensitive(false);
		entry_tool_change_z->set_sensitive(false);
	}
	//tool_change_x_pos
	std::stringstream tool_change_x_pos;
	tool_change_x_pos<<conf.getToolChangeXPos();
	entry_tool_change_x->set_text(tool_change_x_pos.str());
	//tool_change_y_pos
	std::stringstream tool_change_y_pos;
	tool_change_y_pos<<conf.getToolChangeYPos();
	entry_tool_change_y->set_text(tool_change_y_pos.str());
	//tool_change_z_pos
	std::stringstream tool_change_z_pos;
	tool_change_z_pos<<conf.getToolChangeZPos();
	entry_tool_change_z->set_text(tool_change_z_pos.str());
	// use tool sensor
	check_tool_sensor->set_active(conf.getUseToolSensor());
	if(!conf.getUseToolSensor())
	{
		entry_tool_sensor_x->set_sensitive(false);
		entry_tool_sensor_y->set_sensitive(false);
		entry_tool_sensor_z->set_sensitive(false);
		entry_tool_sensor_height->set_sensitive(false);
		entry_touchoff_zpos->set_sensitive(false);
		entry_probe_feed_rate->set_sensitive(false);
	}
	//tool_sensor_x_pos
	std::stringstream tool_sensor_x_pos;
	tool_sensor_x_pos<<conf.getToolSensorXPos();
	entry_tool_sensor_x->set_text(tool_sensor_x_pos.str());
	//tool_sensor_y_pos
	std::stringstream tool_sensor_y_pos;
	tool_sensor_y_pos<<conf.getToolSensorYPos();
	entry_tool_sensor_y->set_text(tool_sensor_y_pos.str());
	//tool_sensor_z_pos
	std::stringstream tool_sensor_z_pos;
	tool_sensor_z_pos<<conf.getToolSensorZPos();
	entry_tool_sensor_z->set_text(tool_sensor_z_pos.str());
	// sensor heigth
	std::stringstream heigth;
	heigth<<conf.getToolSensorHeigth();
	entry_tool_sensor_height->set_text(heigth.str());
	
	//touch off zpos
	std::stringstream touchoff_zpos;
	touchoff_zpos<<conf.getToolSensorTouchOffZPos();
	entry_touchoff_zpos->set_text(touchoff_zpos.str());
	//feed rate
	std::stringstream feed;
	feed<<conf.getToolSensorFeedRate();
	entry_probe_feed_rate->set_text(feed.str());
	//add_exit_pos
	check_add_exit_pos->set_active(conf.getAddExitPos());
	if(!conf.getAddExitPos())
	{
		entry_exit_pos_x->set_sensitive(false);
		entry_exit_pos_y->set_sensitive(false);
		entry_exit_pos_z->set_sensitive(false);
	}
	//exit_pos_x
	std::stringstream exit_pos_x;
	exit_pos_x<<conf.getExitXPos();
	entry_exit_pos_x->set_text(exit_pos_x.str());
	//exit_pos_y
	std::stringstream exit_pos_y;
	exit_pos_y<<conf.getExitYPos();
	entry_exit_pos_y->set_text(exit_pos_y.str());
	//exit_pos_z
	std::stringstream exit_pos_z;
	exit_pos_z<<conf.getExitZPos();
	entry_exit_pos_z->set_text(exit_pos_z.str());
}

void DialogSetting::fillComboUnit(const UNIT u)
{
	bool was_created_before=true;
	if(!m_refTreeModelUnit)
	{
		m_refTreeModelUnit = Gtk::ListStore::create(m_ColumnsUnit);
		was_created_before=false;
	}
	m_refTreeModelUnit->clear();
	combo_units->set_model(m_refTreeModelUnit);
	Gtk::TreeModel::Row row = *(m_refTreeModelUnit->append());
	row[m_ColumnsUnit.m_col_id] = 1;
	ConfigSettings cs_temp;
	cs_temp.setUnit(UNIT_MM);
	row[m_ColumnsUnit.m_col_name] = cs_temp.getLengthUnit();
	row = *(m_refTreeModelUnit->append());
	row[m_ColumnsUnit.m_col_id] = 2;
	cs_temp.setUnit(UNIT_INCH);
	row[m_ColumnsUnit.m_col_name] = cs_temp.getLengthUnit();
	if(!was_created_before)
		combo_units->pack_start(m_ColumnsUnit.m_col_name);
	combo_units->set_active(u);
	combo_units->show_all_children();	
}

void DialogSetting::getSettings(ConfigSettings& conf)
{
	double d;
	//units
	conf.setUnit((UNIT)combo_units->get_active_row_number());
	//add_tool_change_pos
	conf.setAddToolChangePos(check_tool_change->get_active());
	//tool_change_x_pos
	std::stringstream tool_change_x_pos;
	tool_change_x_pos << entry_tool_change_x->get_text();
	tool_change_x_pos>>d;
	conf.setToolChangeXPos(d);
	//tool_change_y_pos
	std::stringstream tool_change_y_pos;
	tool_change_y_pos << entry_tool_change_y->get_text();
	tool_change_y_pos>>d;
	conf.setToolChangeYPos(d);
	//tool_change_z_pos
	std::stringstream tool_change_z_pos;
	tool_change_z_pos << entry_tool_change_z->get_text();
	tool_change_z_pos>>d;
	conf.setToolChangeZPos(d);
	// use tool sensor
	conf.setUseToolSensor(check_tool_sensor->get_active());
	//tool_sensor_x_pos
	std::stringstream tool_sensor_x_pos;
	tool_sensor_x_pos << entry_tool_sensor_x->get_text();
	tool_sensor_x_pos>>d;
	conf.setToolSensorXPos(d);
	//tool_sensor_y_pos
	std::stringstream tool_sensor_y_pos;
	tool_sensor_y_pos << entry_tool_sensor_y->get_text();
	tool_sensor_y_pos>>d;
	conf.setToolSensorYPos(d);
	//tool_sensor_z_pos
	std::stringstream tool_sensor_z_pos;
	tool_sensor_z_pos << entry_tool_sensor_z->get_text();
	tool_sensor_z_pos>>d;
	conf.setToolSensorZPos(d);
	//sensor height
	std::stringstream height;
	height << entry_tool_sensor_height->get_text();
	height>>d;
	conf.setToolSensorHeigth(d);
	//touch off zpos
	std::stringstream touchoff_zpos;
	touchoff_zpos << entry_touchoff_zpos->get_text();
	touchoff_zpos>>d;
	conf.setToolSensorTouchOffZPos(d);
	//feed rate
	std::stringstream feed;
	feed<<entry_probe_feed_rate->get_text();
	feed>>d;
	conf.setToolSensorFeedRate(d);
	//add_exit_pos
	conf.setAddExitPos(check_add_exit_pos->get_active());
	//exit_pos_x
	std::stringstream exit_pos_x;
	exit_pos_x << entry_exit_pos_x->get_text();
	exit_pos_x>>d;
	conf.setExitXPos(d);
	//exit_pos_y
	std::stringstream exit_pos_y;
	exit_pos_y << entry_exit_pos_y->get_text();
	exit_pos_y>>d;
	conf.setExitYPos(d);
	//exit_pos_z
	std::stringstream exit_pos_z;
	exit_pos_z << entry_exit_pos_z->get_text();
	exit_pos_z>>d;
	conf.setExitZPos(d);
}

void DialogSetting::on_check_tool_sensor_toggled()
{
	bool sensitive=check_tool_sensor->get_active();
	entry_tool_sensor_x->set_sensitive(sensitive);
	entry_tool_sensor_y->set_sensitive(sensitive);
	entry_tool_sensor_z->set_sensitive(sensitive);
	entry_tool_sensor_height->set_sensitive(sensitive);
	entry_touchoff_zpos->set_sensitive(sensitive);
	entry_probe_feed_rate->set_sensitive(sensitive);
}

void DialogSetting::on_check_tool_change_toggled()
{
	bool sensitive=check_tool_change->get_active();
	entry_tool_change_x->set_sensitive(sensitive);
	entry_tool_change_y->set_sensitive(sensitive);
	entry_tool_change_z->set_sensitive(sensitive);
}

void DialogSetting::on_check_add_exit_pos_toggled()
{
	bool sensitive=check_add_exit_pos->get_active();
	entry_exit_pos_x->set_sensitive(sensitive);
	entry_exit_pos_y->set_sensitive(sensitive);
	entry_exit_pos_z->set_sensitive(sensitive);
}

void DialogSetting::on_combo_units_changed()
{
	ConfigSettings cs_temp;
	cs_temp.setUnit((UNIT)combo_units->get_active_row_number());
	label_tool_change_pos_unit->set_text(cs_temp.getLengthUnit());
	label_tool_sensor_pos_unit->set_text(cs_temp.getLengthUnit());
	label_unit_length->set_text(cs_temp.getLengthUnit());
	label_tool_sensor_height_unit->set_text(cs_temp.getLengthUnit());
	label_touchoff_unit->set_text(cs_temp.getLengthUnit());
	label_unit_speed->set_text(cs_temp.getSpeedUnit());
	label_probe_feed_rate_unit->set_text(cs_temp.getSpeedUnit());
	label_exit_pos_unit->set_text(cs_temp.getLengthUnit());
}
