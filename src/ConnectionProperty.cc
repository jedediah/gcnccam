/*
	File: ConnectionProperty.cc
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

#include "ConnectionProperty.h"
static const std::string C_CON_PROPERTY_NAME="connection";

const std::string ConnectionProperty::CON_PROPERTY_NAME()
{
	return C_CON_PROPERTY_NAME;
}


ConnectionProperty::ConnectionProperty() : LayerProperty(NULL)
{
	m_name=ConnectionProperty::CON_PROPERTY_NAME();
	m_cut_position=CENTER;
}

ConnectionProperty::ConnectionProperty(const ConnectionProperty& cp)
	:LayerProperty(cp)
{
	m_tools=NULL;
	m_name=ConnectionProperty::CON_PROPERTY_NAME();
	m_color=cp.m_color;
	m_priority=0;
	m_cut_direction=cp.m_cut_direction;
	m_cut_position=CENTER;
	m_cut_feed_rate=cp.m_cut_feed_rate;
	m_z_start_position=cp.m_z_start_position;
	m_cut_depth=0.0;
	m_z_pitch=0.0;
	m_z_feed_rate=0.0;
	m_force_drill_smaller=false;
	m_spindle_speed=0.0;
	m_cooling_mist=false;
	m_cooling_flood=false;
	m_comment=cp.m_comment;
}
		
ConnectionProperty::~ConnectionProperty()
{
	// TODO: put destructor code here
}

LayerProperty* ConnectionProperty::clone(void) const
{
	return new ConnectionProperty(*this);
}

ConnectionProperty& ConnectionProperty::operator=(const ConnectionProperty& cp)
{
	m_tools=NULL;
	m_name=ConnectionProperty::CON_PROPERTY_NAME();
	m_color=cp.m_color;
	m_priority=0;
	m_cut_direction=cp.m_cut_direction;
	m_cut_position=CENTER;
	m_cut_feed_rate=cp.m_cut_feed_rate;
	m_z_start_position=cp.m_z_start_position;
	m_cut_depth=0.0;
	m_z_pitch=0.0;
	m_z_feed_rate=0.0;
	m_force_drill_smaller=false;
	m_spindle_speed=0.0;
	m_cooling_mist=false;
	m_cooling_flood=false;
	m_comment=cp.m_comment;
	return *this;
}
int ConnectionProperty::getPriority() const
{
	return m_priority;
}

void ConnectionProperty::setPriority(const int p)
{
	m_priority=0;
}

cutposition ConnectionProperty::getCutPosition() const
{
	return m_cut_position;
}

void ConnectionProperty::setCutPosition(const cutposition p)
{
	m_cut_position=CENTER;
}

double ConnectionProperty::getCutDepth() const
{
	return m_cut_depth;
}

void ConnectionProperty::setCutDepth(const double d)
{
	m_cut_depth=d;
}

double ConnectionProperty::getZPitch() const
{
	return m_z_pitch;
}

void ConnectionProperty::setZPitch(const double p)
{
	m_z_pitch=0.0;
}

double ConnectionProperty::getZFeedRate() const
{
	return m_z_feed_rate;
}

void ConnectionProperty::setZFeedRate(const double f)
{
	m_z_feed_rate=0.0;
}

bool ConnectionProperty::getForceDrillSmaller() const
{
	return m_force_drill_smaller;
}

void ConnectionProperty::setForceDrillSmaller(const bool f) 
{
	m_force_drill_smaller=false;
}

Tool* ConnectionProperty::getTool() const
{
	return NULL;
}

void ConnectionProperty::setToolNumber(const int n)
{
	m_tool_number=LayerProperty::NO_TOOL();	
}

int ConnectionProperty::getToolNumber() const
{
	return m_tool_number;
}

double ConnectionProperty::getSpindleSpeed() const
{
	return m_spindle_speed;
}

void ConnectionProperty::setSpindleSpeed(const double s)
{
	m_spindle_speed=0.0;
}

bool ConnectionProperty::getCoolingMist() const
{
	return m_cooling_mist;
}

void ConnectionProperty::setCoolingMist(const bool c)
{
	m_cooling_mist=false;
}

bool ConnectionProperty::getCoolingFlood() const
{
	return m_cooling_flood;
}

void ConnectionProperty::setCoolingFlood(const bool c)
{
	m_cooling_flood=false;
}
