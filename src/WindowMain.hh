/*
	File: WindowMain.hh
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

#ifndef _WINDOWMAIN_HH
#  include "WindowMain_glade.hh"
#  define _WINDOWMAIN_HH
#include "FrameTool.hh"
#include "FrameLayerProperty.hh"
#include "ToolList.h"
#include "LayerPropertyList.h"
//#include "LayerList.h"
#include "CadDrawingArea.h"
#include "ConfigSettings.h"
#include <gtkmm/treestore.h>
#include <exception>
#include <stdexcept>
#include <string>
#include <sstream>

#define CONFIG_FOLDER ".gcnccam"
#define CONFIG_FILE "gcnccam.cnf"
#define DEFAULT_TEMPLATE_FILE "gcnccam.gxt"


enum notepads { PAD_DRAWING=0, PAD_PROPERTIES=1, PAD_TOOLS=2 };

class WindowMain : public WindowMain_glade
{  
	public:
		WindowMain();
		~WindowMain();
		bool on_tool_changed(Tool& old_tool, Tool& new_tool);
		GlademmData *getGmmData();
	protected:
		//Tree model columns:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  	public:
	
		ModelColumns()
		{ 
			add(m_col_icon);
			add(m_col_id); 
			add(m_col_name); 
			add(m_col_selectable);
		}
		Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> > m_col_icon;
		Gtk::TreeModelColumn<int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
		Gtk::TreeModelColumn<bool> m_col_selectable;
  };
  ModelColumns m_ColumnsTool;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModelTool;
  ModelColumns m_ColumnsLayerProps;
  Glib::RefPtr<Gtk::TreeStore> m_refTreeModelLayerProps;
  ModelColumns m_ColumnsLayers;
  Glib::RefPtr<Gtk::TreeStore> m_refTreeModelLayers;
	
    private:    
		void on_open_template_activate();
        void on_save_template_activate();
        void on_open_drawing_activate();
        void on_save_cnc_file_activate();
		void on_export_tool_file1_activate();
        void on_quit1_activate();
        void on_save_template_default_activate();
        void on_settings_activate();
		void on_build_cnc_code_activate();
		void on_about_activate();
     	//
		void on_button_open_drawing_clicked();
        void on_button_save_cnc_code_clicked();
        void on_button_build_cnc_clicked();
        void on_button_layerprop_add_clicked();
        void on_button_layerprop_del_clicked();
        void on_button_tool_add_clicked();
        void on_button_tool_del_clicked();
		//
		void on_tree_tools_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
		void on_tree_layerprops_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
		void on_tree_layers_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
		void on_notebook1_switch_page(GtkNotebookPage *p0, guint p1);
		//custom signal handle
		bool on_layer_properties_data_changed(LayerProperty& old_lp, LayerProperty& new_lp);
		void loadConfig();
		void saveConfig();
		void loadTemplate(std::istream &is);
		void saveTemplate(std::ostream &os);
		void fillTreeTools();
		void fillTreeLayerProps();
		void fillTreeLayers();
		void reloadLayers();
		void loadImages();
		//
		
		// vars
		Gtk::Widget* m_active_frame;
		ToolList* m_tools;
		LayerPropertyList* m_layerprops;
		LayerList* m_layers;
		std::string m_filename_drawing;
		
		std::stringstream* m_cnc_stream;
		//std::string m_working_folder;
		//std::string m_template_folder;
		ConfigSettings m_config_settings;
		sigc::connection m_con_layer_properties_data_changed;
		Glib::RefPtr<Gdk::Pixbuf> img_layer16;
		Glib::RefPtr<Gdk::Pixbuf> img_layerprop16;
		Glib::RefPtr<Gdk::Pixbuf> img_tool16;
		
		Gtk::Image* img_layerprop20;
		Gtk::Image* img_layerprop_del20;
		Gtk::Image* img_tool20;
		Gtk::Image* img_tool_del20;
};		
#endif
