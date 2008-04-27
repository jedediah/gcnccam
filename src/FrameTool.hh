/*
	File: FrameTool.hh
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

#ifndef _FRAMETOOL_HH
#  include "FrameTool_glade.hh"
#  define _FRAMETOOL_HH
#include "WindowMain.hh"
#include "Tool.h"
#include "ConfigSettings.h"

class WindowMain; //forward declaration
class FrameTool : public FrameTool_glade
{  
public:
        
        FrameTool(GlademmData *gmm_data, Tool& t, WindowMain* const main_win, ConfigSettings& config);
      
private:
		void on_entry_tool_number_changed();
		void on_button_ok_clicked();
        void on_button_cancel_clicked();
		void fillData(const Tool& t);
		void getData(Tool& t);
		// vars
		WindowMain* m_main_window;
		Tool m_tool_before;
		Tool m_tool_actual;
		ConfigSettings m_config_settings;
};
#endif
