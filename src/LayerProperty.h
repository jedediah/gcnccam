/*
	File: LayerProperty.h
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

#ifndef _LAYERPROPERTY_H_
#define _LAYERPROPERTY_H_
#include <string>
#include <iostream>
#include <gdkmm/color.h>
#include "ToolList.h"
#include "Tool.h"

enum cutposition { OUTSIDE, INSIDE, CENTER };
enum cutdirection { CCW, CW };

class LayerProperty
{
	public:
		LayerProperty(ToolList* tl);
		LayerProperty(const LayerProperty& lp);
		virtual ~LayerProperty();
		virtual LayerProperty* clone(void) const;
		LayerProperty& operator=(const LayerProperty& lp);
		//
		static const int NO_TOOL();
		//
		////void setToolList(ToolList* tl);
		// get/set	
		virtual std::string getName() const;
		virtual void setName(const std::string& n);
		virtual Gdk::Color getColor() const;
		virtual void setColor(const Gdk::Color& c);
		virtual int getPriority() const;
		virtual void setPriority(const int p);
		virtual cutdirection getCutDirection() const;
		virtual void setCutDirection(const cutdirection d);
		virtual cutposition getCutPosition() const;
		virtual void setCutPosition(const cutposition p);
		virtual double getCutFeedRate() const;
		virtual void setCutFeedRate(const double f);
		virtual double getZStartPosition() const;
		virtual void setZStartPosition(const double p);
		virtual double getCutDepth() const;
		virtual void setCutDepth(const double d);
		virtual double getZPitch() const;
		virtual void setZPitch(const double p);
		virtual double getZFeedRate() const;
		virtual void setZFeedRate(const double f);
		virtual bool getForceDrillSmaller() const;
		virtual void setForceDrillSmaller(const bool f); 
		virtual Tool* getTool() const;
		virtual void setToolNumber(const int t);
		virtual int getToolNumber() const;
		virtual double getSpindleSpeed() const;
		virtual void setSpindleSpeed(const double s);
		virtual bool getCoolingMist() const;
		virtual void setCoolingMist(const bool c); 
		virtual bool getCoolingFlood() const;
		virtual void setCoolingFlood(const bool c); 
		virtual std::string getComment() const;
		virtual void setComment(const std::string& c);
		virtual bool cutRight();
		virtual bool cutLeft();		
		//
		//friends
		friend std::istream& operator>>(std::istream &is, LayerProperty& obj);
		friend std::ostream& operator<<(std::ostream &os, const LayerProperty& obj); 

	protected:
		std::string m_name;
		Gdk::Color m_color;
		int m_priority;
		cutdirection m_cut_direction;
		cutposition m_cut_position;
		double m_cut_feed_rate;
		double m_z_start_position;
		double m_cut_depth;
		double m_z_pitch;
		double m_z_feed_rate;
		bool m_force_drill_smaller;
		int m_tool_number;
		double m_spindle_speed;
		bool m_cooling_mist;
		bool m_cooling_flood;
		std::string m_comment;
		ToolList* m_tools;
	
};


#endif	//_LAYERPROPERTY_H_
