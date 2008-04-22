/*
    File: ConfigSettings.cc
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

#include "ConfigSettings.h"
#include <iostream>
#include <sstream>
#include <assert.h>

ConfigSettings::ConfigSettings()
{
	m_working_folder="";
	m_template_folder="";
	m_unit=UNIT_MM;
	m_add_tool_change_pos=false;
	m_tool_change_x_pos=0.0;
	m_tool_change_y_pos=0.0;
	m_tool_change_z_pos=0.0;
	m_use_tool_sensor=false;
	m_tool_sensor_x_pos=0.0;
	m_tool_sensor_y_pos=0.0;
	m_tool_sensor_z_pos=0.0;
	m_tool_sensor_heigth=0.0;
	m_tool_sensor_touchoff_z_pos=0.0;
	m_tool_sensor_probe_feed_rate=0.0;
	m_add_exit_pos=false;
	m_exit_x_pos=0.0;
	m_exit_y_pos=0.0;
	m_exit_z_pos=0.0;
}

ConfigSettings::ConfigSettings(const ConfigSettings& conf)
{
	m_working_folder=conf.getWorkingFolder();
	m_template_folder=conf.getTemplateFolder();
	m_unit=conf.getUnit();
	m_add_tool_change_pos=conf.getAddToolChangePos();
	m_tool_change_x_pos=conf.getToolChangeXPos();
	m_tool_change_y_pos=conf.getToolChangeYPos();
	m_tool_change_z_pos=conf.getToolChangeZPos();
	m_use_tool_sensor=conf.getUseToolSensor();
	m_tool_sensor_x_pos=conf.getToolSensorXPos();
	m_tool_sensor_y_pos=conf.getToolSensorYPos();
	m_tool_sensor_z_pos=conf.getToolSensorZPos();
	m_tool_sensor_heigth=conf.getToolSensorHeigth();
	m_tool_sensor_touchoff_z_pos=conf.getToolSensorTouchOffZPos();
	m_tool_sensor_probe_feed_rate=conf.getToolSensorFeedRate();
	m_add_exit_pos=conf.getAddExitPos();
	m_exit_x_pos=conf.getExitXPos();
	m_exit_y_pos=conf.getExitYPos();
	m_exit_z_pos=conf.getExitZPos();
}
		
ConfigSettings::~ConfigSettings()
{
}

ConfigSettings& ConfigSettings::operator=(const ConfigSettings& conf)
{
	m_working_folder=conf.getWorkingFolder();
	m_template_folder=conf.getTemplateFolder();
	m_unit=conf.getUnit();
	m_add_tool_change_pos=conf.getAddToolChangePos();
	m_tool_change_x_pos=conf.getToolChangeXPos();
	m_tool_change_y_pos=conf.getToolChangeYPos();
	m_tool_change_z_pos=conf.getToolChangeZPos();
	m_use_tool_sensor=conf.getUseToolSensor();
	m_tool_sensor_x_pos=conf.getToolSensorXPos();
	m_tool_sensor_y_pos=conf.getToolSensorYPos();
	m_tool_sensor_z_pos=conf.getToolSensorZPos();
	m_tool_sensor_heigth=conf.getToolSensorHeigth();
	m_tool_sensor_touchoff_z_pos=conf.getToolSensorTouchOffZPos();
	m_tool_sensor_probe_feed_rate=conf.getToolSensorFeedRate();
	m_add_exit_pos=conf.getAddExitPos();
	m_exit_x_pos=conf.getExitXPos();
	m_exit_y_pos=conf.getExitYPos();
	m_exit_z_pos=conf.getExitZPos();
	return *this;
}

bool ConfigSettings::operator==(const ConfigSettings& conf) const
{
	bool ret=false;
	if(m_working_folder.compare(conf.getWorkingFolder())==0
		&& m_template_folder.compare(conf.getTemplateFolder())==0
		&& m_unit==conf.getUnit()
		&& m_add_tool_change_pos==conf.getAddToolChangePos()
		&& m_tool_change_x_pos==conf.getToolChangeXPos()
		&& m_tool_change_y_pos==conf.getToolChangeYPos()
		&& m_tool_change_z_pos==conf.getToolChangeZPos()
		&& m_use_tool_sensor==conf.getUseToolSensor()
		&& m_tool_sensor_x_pos==conf.getToolSensorXPos()
		&& m_tool_sensor_y_pos==conf.getToolSensorYPos()
		&& m_tool_sensor_z_pos==conf.getToolSensorZPos()
		&& m_tool_sensor_heigth==conf.getToolSensorHeigth()
		&& m_tool_sensor_touchoff_z_pos==conf.getToolSensorTouchOffZPos()
		&& m_tool_sensor_probe_feed_rate==conf.getToolSensorFeedRate()
		&& m_add_exit_pos==conf.getAddExitPos()
		&& m_exit_x_pos==conf.getExitXPos()
		&& m_exit_y_pos==conf.getExitYPos()
		&& m_exit_z_pos==conf.getExitZPos())
	{
		ret=true;
	}
	return ret;
}

bool ConfigSettings::operator!=(const ConfigSettings& conf) const
{
	return !operator==(conf);
}

std::string ConfigSettings::getWorkingFolder() const
{
	return m_working_folder;
}

void ConfigSettings::setWorkingFolder(std::string f)
{
	m_working_folder=f;
}

std::string ConfigSettings::getTemplateFolder() const
{
	return m_template_folder;
}

void ConfigSettings::setTemplateFolder(std::string f)
{
	m_template_folder=f;
}

UNIT ConfigSettings::getUnit() const
{
	return m_unit;
}

void ConfigSettings::setUnit(UNIT u)
{
	m_unit=u;
}

std::string ConfigSettings::getLengthUnit() const
{
	std::string unit="mm";
	if(m_unit==UNIT_INCH)
		unit="inch";
	return unit;		
}

std::string ConfigSettings::getSpeedUnit() const
{
	std::string unit="mm/min";
	if(m_unit==UNIT_INCH)
		unit="inch/min";
	return unit;		

}

bool ConfigSettings::getAddToolChangePos() const
{
	return m_add_tool_change_pos;
}
void ConfigSettings::setAddToolChangePos(bool a)
{
	m_add_tool_change_pos=a;
}
		
double ConfigSettings::getToolChangeXPos() const
{
	return m_tool_change_x_pos;
}

void ConfigSettings::setToolChangeXPos(double p)
{
	m_tool_change_x_pos=p;
}

double ConfigSettings::getToolChangeYPos() const
{
	return m_tool_change_y_pos;
}

void ConfigSettings::setToolChangeYPos(double p)
{
	m_tool_change_y_pos=p;
}

double ConfigSettings::getToolChangeZPos() const
{
	return m_tool_change_z_pos;
}

void ConfigSettings::setToolChangeZPos(double p)
{
	m_tool_change_z_pos=p;
}

bool ConfigSettings::getUseToolSensor() const
{
	return m_use_tool_sensor;
}

void ConfigSettings::setUseToolSensor(bool u)
{
	m_use_tool_sensor=u;
}

double ConfigSettings::getToolSensorXPos() const
{
	return m_tool_sensor_x_pos;
}

void ConfigSettings::setToolSensorXPos(double p)
{
	m_tool_sensor_x_pos=p;
}

double ConfigSettings::getToolSensorYPos() const
{
	return m_tool_sensor_y_pos;
}

void ConfigSettings::setToolSensorYPos(double p)
{
	m_tool_sensor_y_pos=p;
}

double ConfigSettings::getToolSensorZPos() const
{
	return m_tool_sensor_z_pos;
}

void ConfigSettings::setToolSensorZPos(double p)
{
	m_tool_sensor_z_pos=p;
}

double ConfigSettings::getToolSensorHeigth() const
{
	return m_tool_sensor_heigth;
}

void ConfigSettings::setToolSensorHeigth(double h)
{
	m_tool_sensor_heigth=h;	
}

double ConfigSettings::getToolSensorTouchOffZPos() const
{
	return m_tool_sensor_touchoff_z_pos;
}

void ConfigSettings::setToolSensorTouchOffZPos(double p)
{
	m_tool_sensor_touchoff_z_pos=p;
}

double ConfigSettings::getToolSensorFeedRate() const
{
	return m_tool_sensor_probe_feed_rate;
}

void ConfigSettings::setToolSensorFeedRate(double f)
{
	m_tool_sensor_probe_feed_rate=f;
}

bool ConfigSettings::getAddExitPos() const
{
	return m_add_exit_pos;
}

void ConfigSettings::setAddExitPos(bool a)
{
	m_add_exit_pos=a;
}
		
double ConfigSettings::getExitXPos() const
{
	return m_exit_x_pos;
}

void ConfigSettings::setExitXPos(double p)
{
	m_exit_x_pos=p;
}

double ConfigSettings::getExitYPos() const
{
	return m_exit_y_pos;
}

void ConfigSettings::setExitYPos(double p)
{
	m_exit_y_pos=p;
}

double ConfigSettings::getExitZPos() const
{
	return m_exit_z_pos;
}

void ConfigSettings::setExitZPos(double p)
{
	m_exit_z_pos=p;
}
		
std::istream& operator>>(std::istream &is, ConfigSettings& obj)
{
	do
	{
		std::string line;
		std::getline(is, line);
		std::string::size_type seperator=line.find_first_of("=");
		std::string variable=line.substr(0, seperator);
		std::string value=line.substr(seperator+1);
		//
		int n;
		bool b;
		double d;
		//working_folder
		if(variable.compare("working_folder")==0)
		{
			obj.setWorkingFolder(value);
		}
		//template_folder
		if(variable.compare("template_folder")==0)
		{
			obj.setTemplateFolder(value);
		}
		//unit
		if(variable.compare("unit")==0)
		{
			std::stringstream unit;
			unit << value;
			unit >> n;
			obj.setUnit(UNIT(n));
		}
		//add_change_tool_pos
		if(variable.compare("add_tool_change_pos")==0)
		{
			
			std::stringstream add_change_tool_pos;
			add_change_tool_pos << value;
			add_change_tool_pos >> b;
			obj.setAddToolChangePos(b);
		}
		//tool_change_x_pos
		if(variable.compare("tool_change_x_pos")==0)
		{
			
			std::stringstream tool_change_x_pos;
			tool_change_x_pos << value;
			tool_change_x_pos >> d;
			obj.setToolChangeXPos(d);
		}
		//tool_change_y_pos
		if(variable.compare("tool_change_y_pos")==0)
		{
			
			std::stringstream tool_change_y_pos;
			tool_change_y_pos << value;
			tool_change_y_pos >> d;
			obj.setToolChangeYPos(d);
		}
		//tool_change_z_pos
		if(variable.compare("tool_change_z_pos")==0)
		{
			
			std::stringstream tool_change_z_pos;
			tool_change_z_pos << value;
			tool_change_z_pos >> d;
			obj.setToolChangeZPos(d);
		}
		//use_tool_sensor
		if(variable.compare("use_tool_sensor")==0)
		{
			
			std::stringstream use_tool_sensor;
			use_tool_sensor << value;
			use_tool_sensor >> b;
			obj.setUseToolSensor(b);
		}
		//tool_sensor_x_pos
		if(variable.compare("tool_sensor_x_pos")==0)
		{
			
			std::stringstream tool_sensor_x_pos;
			tool_sensor_x_pos << value;
			tool_sensor_x_pos >> d;
			obj.setToolSensorXPos(d);
		}
		//tool_sensor_y_pos
		if(variable.compare("tool_sensor_y_pos")==0)
		{
			
			std::stringstream tool_sensor_y_pos;
			tool_sensor_y_pos << value;
			tool_sensor_y_pos >> d;
			obj.setToolSensorYPos(d);
		}
		//tool_sensor_z_pos
		if(variable.compare("tool_sensor_z_pos")==0)
		{
			
			std::stringstream tool_sensor_z_pos;
			tool_sensor_z_pos << value;
			tool_sensor_z_pos >> d;
			obj.setToolSensorZPos(d);
		}
		//tool_sensor_height
		if(variable.compare("tool_sensor_height")==0)
		{
			
			std::stringstream tool_sensor_height;
			tool_sensor_height << value;
			tool_sensor_height >> d;
			obj.setToolSensorHeigth(d);
		}
		//tool_sensor_touchoff_z_pos
		if(variable.compare("tool_sensor_touchoff_z_pos")==0)
		{
			
			std::stringstream tool_sensor_touchoff_z_pos;
			tool_sensor_touchoff_z_pos << value;
			tool_sensor_touchoff_z_pos >> d;
			obj.setToolSensorTouchOffZPos(d);
		}
		//tool_sensor_probe_feed_rate
		if(variable.compare("tool_sensor_probe_feed_rate")==0)
		{
			
			std::stringstream tool_sensor_probe_feed_rate;
			tool_sensor_probe_feed_rate << value;
			tool_sensor_probe_feed_rate >> d;
			obj.setToolSensorFeedRate(d);
		}
		//add_exit_pos
		if(variable.compare("add_exit_pos")==0)
		{
			
			std::stringstream add_exit_pos;
			add_exit_pos << value;
			add_exit_pos >> b;
			obj.setAddExitPos(b);
		}
		//exit_x_pos
		if(variable.compare("exit_x_pos")==0)
		{
			
			std::stringstream exit_x_pos;
			exit_x_pos << value;
			exit_x_pos >> d;
			obj.setExitXPos(d);
		}
		//exit_y_pos
		if(variable.compare("exit_y_pos")==0)
		{
			
			std::stringstream exit_y_pos;
			exit_y_pos << value;
			exit_y_pos >> d;
			obj.setExitYPos(d);
		}
		//exit_z_pos
		if(variable.compare("exit_z_pos")==0)
		{
			
			std::stringstream exit_z_pos;
			exit_z_pos << value;
			exit_z_pos >> d;
			obj.setExitZPos(d);
		}
	}
	while(!is.eof());
	return is;
}

std::ostream& operator<<(std::ostream &os, const ConfigSettings& obj)
{
	//working_folder
	os << "working_folder" << "=" << obj.getWorkingFolder() << std::endl;
	//template_folder
	os << "template_folder" << "=" << obj.getTemplateFolder() << std::endl;	
	//unit
	os << "unit" << "=" << obj.getUnit() << std::endl;	
	//add_tool_change_pos
	os << "add_tool_change_pos" << "=" << obj.getAddToolChangePos() << std::endl;	
	//tool_change_x_pos
	os << "tool_change_x_pos" << "=" << obj.getToolChangeXPos() << std::endl;	
	//tool_change_y_pos
	os << "tool_change_y_pos" << "=" << obj.getToolChangeYPos() << std::endl;	
	//tool_change_z_pos
	os << "tool_change_z_pos" << "=" << obj.getToolChangeZPos() << std::endl;	
	//use_tool_sensor
	os << "use_tool_sensor" << "=" << obj.getUseToolSensor() << std::endl;	
	//tool_sensor_x_pos
	os << "tool_sensor_x_pos" << "=" << obj.getToolSensorXPos() << std::endl;	
	//tool_sensor_y_pos
	os << "tool_sensor_y_pos" << "=" << obj.getToolSensorYPos() << std::endl;	
	//tool_sensor_z_pos
	os << "tool_sensor_z_pos" << "=" << obj.getToolSensorZPos() << std::endl;	
	//tool_sensor_height
	os << "tool_sensor_height" << "=" << obj.getToolSensorHeigth() << std::endl;	
	//tool_sensor_touchoff_z_pos
	os << "tool_sensor_touchoff_z_pos" << "=" << obj.getToolSensorTouchOffZPos() << std::endl;	
	//tool_sensor_probe_feed_rate
	os << "tool_sensor_probe_feed_rate" << "=" << obj.getToolSensorFeedRate() << std::endl;	
	//add_exit_pos
	os << "add_exit_pos" << "=" << obj.getAddExitPos() << std::endl;	
	//exit_x_pos
	os << "exit_x_pos" << "=" << obj.getExitXPos() << std::endl;	
	//exit_y_pos
	os << "exit_y_pos" << "=" << obj.getExitYPos() << std::endl;	
	//exit_z_pos
	os << "exit_z_pos" << "=" << obj.getExitZPos() << std::endl;	
	return os;
}
