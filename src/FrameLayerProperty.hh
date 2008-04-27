/*
	File: FrameLayerProperty.hh
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

#ifndef _FRAMELAYERPROPERTY_HH
#  include "FrameLayerProperty_glade.hh"
#  define _FRAMELAYERPROPERTY_HH
#include <gtkmm/colorbutton.h>
#include <gtkmm/liststore.h>
#include "LayerProperty.h"
	
class FrameLayerProperty : public FrameLayerProperty_glade
{  
public:
        
        FrameLayerProperty(GlademmData *gmm_data, LayerProperty& lp, ToolList* tools );
		~FrameLayerProperty();
		typedef sigc::signal<bool, LayerProperty&, LayerProperty&> type_signal_data_changed;
		type_signal_data_changed signal_data_changed();

protected:
  type_signal_data_changed m_signal_data_changed;
		//Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:
	
    ModelColumns()
    { add(m_col_id); add(m_col_name); }

    Gtk::TreeModelColumn<int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
  };

  ModelColumns m_ColumnsDirection;
  ModelColumns m_ColumnsPosition;
  ModelColumns m_ColumnsTool;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModelDirection;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModelPosition;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModelTool;
private:
		void fillData(const LayerProperty& lp);
		void getData(LayerProperty& lp);
		void createColorButton();
		void fillComboDirection(const cutdirection dir);
		void fillComboPosition(const cutposition pos);
		void fillComboTools(Tool* t);
		void on_button_ok_clicked();
        void on_button_cancel_clicked();
		void on_check_cooling_mist_toggled();
		Gtk::ColorButton* m_button_collor;
		ToolList* m_tools;
		LayerProperty* m_lp_before;
		bool m_is_connection;
		
};
#endif
