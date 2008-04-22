/*
	File: DialogSetting.hh
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

#ifndef _DIALOGSETTING_HH
#  include "DialogSetting_glade.hh"
#  define _DIALOGSETTING_HH
#include "ConfigSettings.h"
#include <gtkmm/liststore.h>

class DialogSetting : public DialogSetting_glade
{  
	public:
	DialogSetting(const ConfigSettings& conf);
	virtual	~DialogSetting();
	void getSettings(ConfigSettings& conf);
	private:
		//Tree model columns:
		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
		public:
		
		ModelColumns()
		{ add(m_col_id); add(m_col_name); }
		
		Gtk::TreeModelColumn<int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
		};

  		ModelColumns m_ColumnsUnit;
		Glib::RefPtr<Gtk::ListStore> m_refTreeModelUnit;		
		void fillData(const ConfigSettings& conf);
		void fillComboUnit(const UNIT u);
		virtual void on_check_tool_sensor_toggled();
		virtual void on_check_tool_change_toggled();
		virtual void on_check_add_exit_pos_toggled();
		virtual void on_combo_units_changed();
};
#endif
