/*
	File: LayerProperty.cc
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

#include <gtkmm/colorselection.h>
#include "LayerProperty.h"
#include <assert.h>
#include <sstream>

static const int C_NO_TOOL=-1;

const int LayerProperty::NO_TOOL()
{
	return C_NO_TOOL;
}

LayerProperty::LayerProperty(ToolList* tl)
{
	//assert(tl);
	m_tools=tl;
	m_name="";
	m_color.set("Grey");
	m_priority=0;
	m_cut_direction=CCW;
	m_cut_position=OUTSIDE;
	m_cut_feed_rate=0.0;
	m_z_start_position=0.0;
	m_cut_depth=0.0;
	m_z_pitch=0.0;
	m_z_feed_rate=0.0;
	m_force_drill_smaller=false;
	setToolNumber(LayerProperty::NO_TOOL());
	m_spindle_speed=0.0;
	m_cooling_mist=false;
	m_cooling_flood=false;
	m_comment="";

}

LayerProperty::LayerProperty(const LayerProperty& lp)
{
	m_tools=lp.m_tools;
	//assert(m_tools);
	m_name=lp.m_name;
	m_color=lp.m_color;
	m_priority=lp.m_priority;
	m_cut_direction=lp.m_cut_direction;
	m_cut_position=lp.m_cut_position;
	m_cut_feed_rate=lp.m_cut_feed_rate;
	m_z_start_position=lp.m_z_start_position;
	m_cut_depth=lp.m_cut_depth;
	m_z_pitch=lp.m_z_pitch;
	m_z_feed_rate=lp.m_z_feed_rate;
	m_force_drill_smaller=lp.m_force_drill_smaller;
	setToolNumber(lp.m_tool_number);
	m_spindle_speed=lp.m_spindle_speed;
	m_cooling_mist=lp.m_cooling_mist;
	m_cooling_flood=lp.m_cooling_flood;
	m_comment=lp.m_comment;
	
}
LayerProperty::~LayerProperty()
{
	// TODO: put destructor code here
}

LayerProperty* LayerProperty::clone(void) const
{
	return new LayerProperty(*this);
}

LayerProperty& LayerProperty::operator=(const LayerProperty& lp)
{
	m_name=lp.m_name;
	m_color=lp.m_color;
	m_priority=lp.m_priority;
	m_cut_direction=lp.m_cut_direction;
	m_cut_position=lp.m_cut_position;
	m_cut_feed_rate=lp.m_cut_feed_rate;
	m_z_start_position=lp.m_z_start_position;
	m_cut_depth=lp.m_cut_depth;
	m_z_pitch=lp.m_z_pitch;
	m_z_feed_rate=lp.m_z_feed_rate;
	m_force_drill_smaller=lp.m_force_drill_smaller;
	m_tool_number=lp.m_tool_number;
	m_spindle_speed=lp.m_spindle_speed;
	m_cooling_mist=lp.m_cooling_mist;
	m_cooling_flood=lp.m_cooling_flood;
	m_comment=lp.m_comment;
	m_tools=lp.m_tools;
	return *this;
}
/*
void LayerProperty::setToolList(ToolList* tl)
{
	m_tools=tl;
}
*/
std::string LayerProperty::getName() const
{
	return m_name;
}
void LayerProperty::setName(const std::string& n)
{
	m_name=n;
}
Gdk::Color LayerProperty::getColor() const
{
	return m_color;
}

void LayerProperty::setColor(const Gdk::Color& c)
{
	m_color=c;
}

int LayerProperty::getPriority() const
{
	return m_priority;
}

void LayerProperty::setPriority(const int p)
{
	m_priority=p;
}

cutdirection LayerProperty::getCutDirection() const
{
	return m_cut_direction;
}

void LayerProperty::setCutDirection(const cutdirection d)
{
	m_cut_direction=d;
}

cutposition LayerProperty::getCutPosition() const
{
	return m_cut_position;
}

void LayerProperty::setCutPosition(const cutposition p)
{
	m_cut_position=p;
}

double LayerProperty::getCutFeedRate() const
{
	return m_cut_feed_rate;
}

void LayerProperty::setCutFeedRate(const double f)
{
	m_cut_feed_rate=f;
}

double LayerProperty::getZStartPosition() const
{
	return m_z_start_position;
}

void LayerProperty::setZStartPosition(const double p)
{
	m_z_start_position=p;
}

double LayerProperty::getCutDepth() const
{
	return m_cut_depth;
}

void LayerProperty::setCutDepth(const double d)
{
	m_cut_depth=d;
}

double LayerProperty::getZPitch() const
{
	return m_z_pitch;
}

void LayerProperty::setZPitch(const double p)
{
	m_z_pitch=p;
}



double LayerProperty::getZFeedRate() const
{
	return m_z_feed_rate;
}

void LayerProperty::setZFeedRate(const double f)
{
	m_z_feed_rate=f;
}

bool LayerProperty::getForceDrillSmaller() const
{
	return m_force_drill_smaller;
}

void LayerProperty::setForceDrillSmaller(const bool f) 
{
	m_force_drill_smaller=f;
}
		

Tool* LayerProperty::getTool() const
{
	//assert(m_tools);
	
	Tool* ret=NULL;
	if(m_tools!=NULL)
		ret=m_tools->find(m_tool_number);
	return ret;
}

void LayerProperty::setToolNumber(const int n)
{
	m_tool_number=LayerProperty::NO_TOOL();	
	if(m_tools!=NULL && m_tools->find(n))
	{
		m_tool_number=n;
	}
}

int LayerProperty::getToolNumber() const
{
	int ret=LayerProperty::NO_TOOL();
	if(getTool()!=NULL)
	{
		ret=m_tool_number;
	}
	return ret;
}

double LayerProperty::getSpindleSpeed() const
{
	return m_spindle_speed;
}

void LayerProperty::setSpindleSpeed(const double s)
{
	m_spindle_speed=s;
}

bool LayerProperty::getCoolingMist() const
{
	return m_cooling_mist;
}

void LayerProperty::setCoolingMist(const bool c)
{
	m_cooling_mist=c;
	if(!m_cooling_mist);
	{
		m_cooling_flood=false;
	}
}

bool LayerProperty::getCoolingFlood() const
{
	return m_cooling_flood;
}

void LayerProperty::setCoolingFlood(const bool c)
{
	m_cooling_flood=c;
	if(m_cooling_flood);
	{
		m_cooling_mist=true;
	}
}
 		
std::string LayerProperty::getComment() const
{
	return m_comment;
}
void LayerProperty::setComment(const std::string& c)
{
	m_comment=c;
}

bool LayerProperty::cutRight()
{
	bool ret = false;
	if(m_cut_position!=CENTER)
	{
		if((m_cut_direction == CW && m_cut_position==INSIDE)
			|| (m_cut_direction == CCW && m_cut_position==OUTSIDE))
		{
			ret=true; 
		}
	}
	return ret;
}

bool LayerProperty::cutLeft()
{
	bool ret = false;
	if(m_cut_position!=CENTER)
	{
		if((m_cut_direction == CCW && m_cut_position==INSIDE)
			|| (m_cut_direction == CW && m_cut_position==OUTSIDE))
		{
			ret=true; 
		}
	}
	return ret;
}

std::istream& operator>>(std::istream &is, LayerProperty& obj)
{
	do
	{
		std::string line;
		std::getline(is, line);
		std::string::size_type seperator=line.find_first_of("=");
		std::string variable=line.substr(0, seperator);
		std::string value=line.substr(seperator+1);
		int n;
		double d;
		bool b;
		ushort c_val;
		//NAME
		if(variable.compare("name")==0)
		{
			obj.setName(value);
		}
		//RED
		if(variable.compare("red")==0)
		{
			std::stringstream red;
			red << value;
			red >> c_val;
			Gdk::Color color = obj.getColor();
			color.set_red(c_val);
			obj.setColor(color);	
		}
		//GREEN
		if(variable.compare("green")==0)
		{
			std::stringstream green;
			green << value;
			green >> c_val;
			Gdk::Color color = obj.getColor();
			color.set_green(c_val);
			obj.setColor(color);	
		}
		//BLUE
		if(variable.compare("blue")==0)
		{
			std::stringstream blue;
			blue << value;
			blue >> c_val;
			Gdk::Color color = obj.getColor();
			color.set_blue(c_val);
			obj.setColor(color);	
		}
		//PRIORITY
		if(variable.compare("priority")==0)
		{
			std::stringstream priority;
			priority << value;
			priority >> n;
			obj.setPriority(n);	
		}
		//DIRECTION
		if(variable.compare("direction")==0)
		{
			std::stringstream direction;
			direction << value;
			direction >> n;
			obj.setCutDirection((cutdirection)n);	
		}
		//POSITION
		if(variable.compare("position")==0)
		{
			std::stringstream position;
			position << value;
			position >> n;
			obj.setCutPosition(cutposition(n));	
		}
		//FEED_RATE
		if(variable.compare("feed_rate")==0)
		{
			std::stringstream feed_rate;
			feed_rate << value;
			feed_rate >> d;
			obj.setCutFeedRate(d);	
		}
		//Z_START_POSITON
		if(variable.compare("z_start_position")==0)
		{
			std::stringstream z_start_position;
			z_start_position << value;
			z_start_position >> d;
			obj.setZStartPosition(d);
		}
		//CUT_DEPTH
		if(variable.compare("cut_depth")==0)
		{
			std::stringstream cut_depth;
			cut_depth << value;
			cut_depth >> d;
			obj.setCutDepth(d);
		}
		//ZPITCH
		if(variable.compare("z_pitch")==0)
		{
			std::stringstream z_pitch;
			z_pitch << value;
			z_pitch >> d;
			obj.setZPitch(d);
		}
		//Z_FFED_RATE
		if(variable.compare("z_feed_rate")==0)
		{
			std::stringstream z_feed_rate;
			z_feed_rate << value;
			z_feed_rate >> d;
			obj.setZFeedRate(d);
		}
		//DRILL_SMALLER
		if(variable.compare("force_drill_smaller")==0)
		{
			std::stringstream force_drill_smaller;
			force_drill_smaller << value;
			force_drill_smaller >> b;
			obj.setForceDrillSmaller(b);
		}
		//TOOL_NUMBER
		if(variable.compare("tool_number")==0)
		{
			std::stringstream tool_number;
			tool_number << value;
			tool_number >> n;
			obj.setToolNumber(n);
		}
		//SPINDLE_SPEED
		if(variable.compare("spindle_speed")==0)
		{
			std::stringstream spindle_speed;
			spindle_speed << value;
			spindle_speed >> d;
			obj.setSpindleSpeed(d);
		}
		//COOLING_MIST
		if(variable.compare("cooling_mist")==0)
		{
			std::stringstream cooling_mist;
			cooling_mist << value;
			cooling_mist >> b;
			obj.setCoolingMist(b);
		}
		//COOLING_FLOOD
		if(variable.compare("cooling_flood")==0)
		{
			std::stringstream cooling_flood;
			cooling_flood << value;
			cooling_flood >> b;
			obj.setCoolingFlood(b);
		}
		//COMMENT
		if(variable.compare("comment")==0)
		{
			obj.setComment(value);
		}
	}
	while(is.good());
	return is;
}


	
std::ostream& operator<<(std::ostream &os, const LayerProperty& obj)
{
	//NAME
	os << "name" << "=" << obj.getName() << std::endl;
	//RED
	os << "red" << "=" << obj.getColor().get_red() << std::endl;
	//GREEN
	os << "green" << "=" << obj.getColor().get_green() << std::endl;
	//BLUE
	os << "blue" << "=" << obj.getColor().get_blue() << std::endl;
	//PRIORITY
	os << "priority" << "=" << obj.getPriority() << std::endl;
	//DIRECTION
	os << "direction" << "=" << obj.getCutDirection() << std::endl;
	//POSITION
	os << "position" << "=" << obj.getCutPosition() << std::endl;
	//FEED_RATE
	os << "feed_rate" << "=" << obj.getCutFeedRate() << std::endl;
	//Z_START_POSITON
	os << "z_start_position" << "=" << obj.getZStartPosition() << std::endl;
	//CUT_DEPTH
	os << "cut_depth" << "=" << obj.getCutDepth() << std::endl;
	//ZPITCH
	os << "z_pitch" << "=" << obj.getZPitch() << std::endl;
	//Z_FFED_RATE
	os << "z_feed_rate" << "=" << obj.getZFeedRate() << std::endl;
	//DRILL_SMALLER
	os << "force_drill_smaller" << "=" << obj.getForceDrillSmaller() << std::endl;
	//TOOL_NUMBER
	os << "tool_number" << "=" << obj.getToolNumber() << std::endl;
	//SPINDLE_SPEED
	os << "spindle_speed" << "=" << obj.getSpindleSpeed() << std::endl;
	//COOLING_MIST
	os << "cooling_mist" << "=" << obj.getCoolingMist() << std::endl;
	//COOLING_FLOOD
	os << "cooling_flood" << "=" << obj.getCoolingFlood() << std::endl;
	//COMMENT
	os << "comment" << "=" << obj.getComment() << std::endl;
	return os;
}
