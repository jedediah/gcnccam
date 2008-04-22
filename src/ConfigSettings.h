/*
    File: ConfigSettings.h
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

#ifndef _CONFIGSETTINGS_H_
#define _CONFIGSETTINGS_H_

#include <string>

enum UNIT { UNIT_MM=0, UNIT_INCH=1 };


//------------------------------------------------------------------------------
// ConfigSettings Declaration
//------------------------------------------------------------------------------


class ConfigSettings
{
	public:
		ConfigSettings();
		ConfigSettings(const ConfigSettings& conf);
		virtual ~ConfigSettings();
		ConfigSettings& operator=(const ConfigSettings& conf);
		bool operator==(const ConfigSettings& conf) const;
		bool operator!=(const ConfigSettings& conf) const;
		std::string getWorkingFolder() const;
		void setWorkingFolder(std::string f);
		std::string getTemplateFolder() const;
		void setTemplateFolder(std::string f);
		UNIT getUnit() const;
		void setUnit(UNIT u);
		std::string getLengthUnit() const;
		std::string getSpeedUnit() const;
		bool getAddToolChangePos() const;
		void setAddToolChangePos(bool a);
		double getToolChangeXPos() const;
		void setToolChangeXPos(double p);
		double getToolChangeYPos() const;
		void setToolChangeYPos(double p);
		double getToolChangeZPos() const;
		void setToolChangeZPos(double p);
		bool getUseToolSensor() const;
		void setUseToolSensor(bool u);
		double getToolSensorXPos() const;
		void setToolSensorXPos(double p);
		double getToolSensorYPos() const;
		void setToolSensorYPos(double p);
		double getToolSensorZPos() const;
		void setToolSensorZPos(double p);
		double getToolSensorHeigth() const;
		void setToolSensorHeigth(double h);
		double getToolSensorTouchOffZPos() const;
		void setToolSensorTouchOffZPos(double p);
		double getToolSensorFeedRate() const;
		void setToolSensorFeedRate(double f);
		bool getAddExitPos() const;
		void setAddExitPos(bool a);
		double getExitXPos() const;
		void setExitXPos(double p);
		double getExitYPos() const;
		void setExitYPos(double p);
		double getExitZPos() const;
		void setExitZPos(double p);
		
		//friends
		friend std::istream& operator>>(std::istream &is, ConfigSettings& obj);
		friend std::ostream& operator<<(std::ostream &os, const ConfigSettings& obj); 
	protected:
		std::string m_working_folder;
		std::string m_template_folder;
		UNIT m_unit;
		bool m_add_tool_change_pos;
		double m_tool_change_x_pos;
		double m_tool_change_y_pos;
		double m_tool_change_z_pos;
		bool m_use_tool_sensor;
		double m_tool_sensor_x_pos;
		double m_tool_sensor_y_pos;
		double m_tool_sensor_z_pos;
		double m_tool_sensor_heigth;
		double m_tool_sensor_touchoff_z_pos;
		double m_tool_sensor_probe_feed_rate;
		bool m_add_exit_pos;
		double m_exit_x_pos;
		double m_exit_y_pos;
		double m_exit_z_pos;
		
	
};

#endif	//_CONFIGSETTINGS_H_
