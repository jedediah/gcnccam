/*
	File: ConnectionProperty.h
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

#ifndef _CONNECTIONPROPERTY_H_
#define _CONNECTIONPROPERTY_H_

#include "LayerProperty.h"

class ConnectionProperty : public LayerProperty
{
	public:
		ConnectionProperty();
		ConnectionProperty(const ConnectionProperty& cp);
		virtual ~ConnectionProperty();
		virtual LayerProperty* clone(void) const;
		ConnectionProperty& operator=(const ConnectionProperty& cp);
		static const std::string CON_PROPERTY_NAME();
		// ConnectionProperty interface
	
		// TODO: add member function declarations...
		virtual int getPriority() const;
		virtual void setPriority(const int p);
		virtual cutposition getCutPosition() const;
		virtual void setCutPosition(const cutposition p);
		virtual double getCutDepth() const;
		virtual void setCutDepth(const double p);
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
		
	protected:
		// ConnectionProperty variables
	
		// TODO: add member variables...
	
};


#endif	//_CONNECTIONPROPERTY_H_
