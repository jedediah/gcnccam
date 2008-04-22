/*
	File: WindowMain.cc
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

#include "config.h"
#include "WindowMain.hh"
#include "DxfReader.h"
#include "GerberNcReader.h"
#include "DialogSetting.hh"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <gdkmm/pixbufloader.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>
#include <gtkmm/textbuffer.h>
#include <gtkmm/aboutdialog.h>
#include <sstream>
#include <map>
#include <string>

WindowMain::WindowMain() :WindowMain_glade()
{
	
	m_active_frame=frame_default;
	m_tools=new ToolList();
	m_layerprops = new LayerPropertyList(m_tools);
	loadImages();
	loadConfig();
	fillTreeTools();
	fillTreeLayerProps();
	m_cnc_stream=NULL;
	save_cnc_file->set_sensitive(false);
	build_cnc_code->set_sensitive(false);
}

WindowMain::~WindowMain()
{
	if(m_layers)
		delete m_layers;
	if(m_layerprops)
		delete m_layerprops;
	if(m_tools)
		delete m_tools;
	if(m_cnc_stream!=NULL)
		delete m_cnc_stream;
	//
	saveConfig();
	
}

void WindowMain::on_open_template_activate()
{
	Gtk::FileChooserDialog dialog("Open Template file", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN,Gtk::RESPONSE_ACCEPT);
	// filter
	Gtk::FileFilter filter_all;
	filter_all.set_name("All Files");
	filter_all.add_pattern("*.*");
	dialog.add_filter(filter_all);
	Gtk::FileFilter filter_templ;
	filter_templ.set_name("Template Files");
	filter_templ.add_pattern("*.gxt");
	dialog.add_filter(filter_templ);
	dialog.set_filter(filter_templ);
	dialog.set_current_folder(m_config_settings.getTemplateFolder());
	int result = dialog.run();
	if( result == Gtk::RESPONSE_ACCEPT )
	{
		if(m_active_frame)
		{
			hpaned1->remove(*m_active_frame);
			delete m_active_frame;
			m_active_frame=NULL;
		}
		if(m_con_layer_properties_data_changed)
		{
			m_con_layer_properties_data_changed.disconnect();
		}
		m_active_frame = Gtk::manage(new class Gtk::Frame());
		hpaned1->pack2(*m_active_frame, Gtk::EXPAND);
		m_active_frame->show();
		
		if(m_layerprops)
			m_layerprops->clear();
		if(m_tools)
			m_tools->clear();	
		std::string filename=dialog.get_filename();
		try
		{
			std::ifstream input_stream;
			input_stream.open(filename.c_str());
			loadTemplate(input_stream);
			input_stream.close();
			m_config_settings.setTemplateFolder(dialog.get_current_folder());
			//
			fillTreeTools();			
			fillTreeLayerProps();
			reloadLayers();
		}
		catch (std::runtime_error& e)
		{
			std::string whatson = e.what();
			Gtk::MessageDialog dlgError(*this, "Error on read Template file!",
										false, Gtk::MESSAGE_ERROR, 
										Gtk::BUTTONS_CANCEL);
			dlgError.set_secondary_text(e.what());
			dlgError.run();
		}

	}
}

void WindowMain::on_open_drawing_activate()
{
	on_button_open_drawing_clicked();
}

void WindowMain::on_save_template_activate()
{
	if(m_tools && m_layerprops)
	{
		Gtk::FileChooserDialog dialog("Save Template file (.gxt)", Gtk::FILE_CHOOSER_ACTION_SAVE);
		dialog.set_transient_for(*this);
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE,Gtk::RESPONSE_OK);
		// filter
		
		Gtk::FileFilter filter_templ;
		filter_templ.set_name("Template Files");
		filter_templ.add_pattern("*.gxt");
		dialog.add_filter(filter_templ);
		dialog.set_filter(filter_templ);
		dialog.set_current_folder(m_config_settings.getTemplateFolder());
		int result = dialog.run();
		if( result == Gtk::RESPONSE_OK )
		{
			std::string file_path=dialog.get_filename();
			
			try
			{
				std::string::size_type ext_seperator=file_path.find_last_of(".");
				std::string extension=file_path.substr(ext_seperator+1);
				if(extension.compare("gxt")!=0)
				{
					file_path+=".gxt";
				}
				///
				std::ofstream output_stream;
				output_stream.open(file_path.c_str());
				if(!output_stream)
				{
					throw(std::runtime_error("File not found!"));
					
				}
				saveTemplate(output_stream);
				output_stream << std::flush;
				output_stream.close();

				///
				std::cout << "save template file file to: " << file_path << std::endl;
				m_config_settings.setTemplateFolder(dialog.get_current_folder());
			}
			catch (std::runtime_error& e)
			{
				std::string whatson = e.what();
				Gtk::MessageDialog dlgError(*this, "Error write write template file!",
											false, Gtk::MESSAGE_ERROR, 
											Gtk::BUTTONS_CANCEL);
				dlgError.set_secondary_text(e.what());
				dlgError.run();
			}	
		}
	}

}

void WindowMain::on_save_cnc_file_activate()
{
	on_button_save_cnc_code_clicked();
}

void WindowMain::on_export_tool_file1_activate()
{
}

void WindowMain::on_quit1_activate()
{
}

void WindowMain::on_save_template_default_activate()
{
	std::string config_folder(g_get_home_dir());
	config_folder.append(1, G_DIR_SEPARATOR);
	config_folder.append(CONFIG_FOLDER);
	//
	std::string templ_file(config_folder);
	templ_file.append(1, G_DIR_SEPARATOR);
	templ_file.append(DEFAULT_TEMPLATE_FILE);
	std::ofstream output_stream;
	output_stream.open(templ_file.c_str());
	if(output_stream)
	{
		saveTemplate(output_stream);
		std::cout << "on_save_template_default_activate: saveTemplate " << std::endl;
	}
	output_stream << std::flush;
	output_stream.close();
}

void WindowMain::on_settings_activate()
{
	DialogSetting dialog(m_config_settings);
	int result=dialog.run();
	if( result == Gtk::RESPONSE_OK )
	{
		dialog.getSettings(m_config_settings);
	}
}
		
void WindowMain::on_build_cnc_code_activate()
{
	on_button_build_cnc_clicked();
}

void WindowMain::on_about_activate()
{
	Gtk::AboutDialog dialog ;
	dialog .set_transient_for( *this ) ; 
	dialog .set_name("gCncCam") ;
	dialog .set_logo( this ->get_icon() ) ;
	dialog .set_version( VERSION ) ;
	dialog .set_comments("GNU CNC CAM drawing to g-code converter") ;
	dialog .set_copyright( "Copyright © 2008 Arnd Begemann" ) ;
	dialog .set_website( "http://gcnccam.sourceforge.net" ) ;
 	dialog .run() ;
}

		
void WindowMain::on_button_open_drawing_clicked()
{
	Gtk::FileChooserDialog dialog("Open DXF file", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN,Gtk::RESPONSE_ACCEPT);
	// filter
	/*
	Gtk::FileFilter filter_all;
	filter_all.set_name("All Files");
	filter_all.add_pattern("*.*");
	dialog.add_filter(filter_all);
	*/
	Gtk::FileFilter filter_dxf;
	filter_dxf.set_name("DXF Files");
	filter_dxf.add_pattern("*.dxf");
	filter_dxf.add_pattern("*.Dxf");
	filter_dxf.add_pattern("*.DXF");
	dialog.add_filter(filter_dxf);
	dialog.set_filter(filter_dxf);
	Gtk::FileFilter filter_gerber_nc;
	filter_gerber_nc.set_name("Gerber NC File");
	filter_gerber_nc.add_pattern("*.cnc");
	dialog.add_filter(filter_gerber_nc);
	
	dialog.set_current_folder(m_config_settings.getWorkingFolder());
	int result = dialog.run();
	if( result == Gtk::RESPONSE_ACCEPT )
	{
		save_cnc_file->set_sensitive(false);
		button_save_cnc_code->set_sensitive(false);
		if(m_active_frame)
		{
			hpaned1->remove(*m_active_frame);
			delete m_active_frame;
			m_active_frame=NULL;
		}
		if(m_con_layer_properties_data_changed)
		{
			m_con_layer_properties_data_changed.disconnect();
		}
		m_active_frame = Gtk::manage(new class Gtk::Frame());
		hpaned1->pack2(*m_active_frame, Gtk::EXPAND);
		m_active_frame->show();
		if(m_layers)
			delete m_layers;
		m_layers=NULL;
		std::string filename=dialog.get_filename();	
		m_layers=new LayerList(m_layerprops, &m_config_settings);
		try
		{
			if(filter_dxf.get_name() == dialog.get_filter()->get_name())
			{
				DxfReader reader(*m_layers);
				reader.readFile(dialog.get_filename());
			}
			else if(filter_gerber_nc.get_name() == dialog.get_filter()->get_name())
			{
				std::cout << "here we go!" << std::endl;
				GerberNcReader reader(*m_layers);
				reader.readFile(dialog.get_filename());
			}
			std::string filepath_drawing=dialog.get_filename();
			std::string::size_type path_seperator=filepath_drawing.find_last_of(G_DIR_SEPARATOR);
			m_filename_drawing=filepath_drawing.substr(path_seperator+1);
			m_layers->calcGraphs();
			fillTreeLayers();
			m_config_settings.setWorkingFolder(dialog.get_current_folder());
			//
			button_build_cnc->set_sensitive(true);
			build_cnc_code->set_sensitive(true);
		}
		catch (std::runtime_error& e)
		{
			std::string whatson = e.what();
			Gtk::MessageDialog dlgError(*this, "Error on read DXF file!",
										false, Gtk::MESSAGE_ERROR, 
										Gtk::BUTTONS_CANCEL);
			dlgError.set_secondary_text(e.what());
			dlgError.run();
		}

	}
}
void WindowMain::on_button_save_cnc_code_clicked()
{
	if(m_cnc_stream)
	{
		Gtk::FileChooserDialog dialog("Save cnc file (.ngc)", Gtk::FILE_CHOOSER_ACTION_SAVE);
		dialog.set_transient_for(*this);
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE,Gtk::RESPONSE_OK);
		// filter
		Gtk::FileFilter filter_cnc;
		filter_cnc.set_name("CNC Files");
		filter_cnc.add_pattern("*.ngc");
		dialog.add_filter(filter_cnc);
		dialog.set_filter(filter_cnc);
		dialog.set_current_folder(m_config_settings.getWorkingFolder());
		std::string::size_type ext_seperator=m_filename_drawing.find_last_of(".");
		std::string current_file_name=m_filename_drawing.substr(0,ext_seperator);
		dialog.set_current_name(current_file_name);
		int result = dialog.run();
		if( result == Gtk::RESPONSE_OK )
		{
			std::string file_path_nc=dialog.get_filename();
			
			try
			{
				ext_seperator=file_path_nc.find_last_of(".");
				std::string extension=file_path_nc.substr(ext_seperator+1);
				if(extension.compare("ngc")!=0)
				{
					file_path_nc+=".ngc";
				}
				///
				std::ofstream output_stream;
				output_stream.open(file_path_nc.c_str());
				if(!output_stream)
				{
					throw(std::runtime_error("File not found!"));
					
				}
				output_stream << m_cnc_stream->str();
				output_stream << std::flush;
				output_stream.close();

				///
				std::cout << "save cnc file to: " << file_path_nc << std::endl;
				m_config_settings.setWorkingFolder(dialog.get_current_folder());
			}
			catch (std::runtime_error& e)
			{
				std::string whatson = e.what();
				Gtk::MessageDialog dlgError(*this, "Error write nc file!",
											false, Gtk::MESSAGE_ERROR, 
											Gtk::BUTTONS_CANCEL);
				dlgError.set_secondary_text(e.what());
				dlgError.run();
			}	
		}
	}
}

void WindowMain::on_button_build_cnc_clicked()
{
	if(m_cnc_stream!=NULL)
	{
		delete m_cnc_stream;
		m_cnc_stream=NULL;
	}
	if(m_layers)
	{
		m_cnc_stream= new std::stringstream();
		m_layers->getCncCode(*m_cnc_stream);
		if(m_active_frame)
		{
			hpaned1->remove(*m_active_frame);
			delete m_active_frame;
			m_active_frame=NULL;
		}
		if(m_con_layer_properties_data_changed)
		{
			m_con_layer_properties_data_changed.disconnect();
		}
		// display cnc code in text view
		Gtk::TextView* text_view = new Gtk::TextView();
		text_view->set_editable(false);
		Gtk::ScrolledWindow* scroll_text_view= Gtk::manage(new Gtk::ScrolledWindow());
		m_active_frame=scroll_text_view;
		scroll_text_view->add(*text_view);
		scroll_text_view->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
		hpaned1->pack2(*m_active_frame, Gtk::EXPAND);
		m_active_frame->show();
		text_view->show();
		Glib::RefPtr<Gtk::TextBuffer> ref_text_buffer;
		ref_text_buffer=Gtk::TextBuffer::create();
		ref_text_buffer->set_text(m_cnc_stream->str());
		text_view->set_buffer(ref_text_buffer);
		//
		save_cnc_file->set_sensitive(true);
		button_save_cnc_code->set_sensitive(true);
	}
}

void WindowMain::on_tree_layers_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
	Glib::RefPtr<Gtk::TreeSelection> selection = tree_layers->get_selection();
	Gtk::TreeModel::iterator it=selection->get_selected();
	button_tool_del->set_sensitive(false);
	button_layerprop_del->set_sensitive(false);
	if(it)
	{
		Gtk::TreeModel::Row row = *it;
		if(row[m_ColumnsLayers.m_col_selectable])		
		{	
			Glib::ustring name=row[m_ColumnsLayers.m_col_name];
			Layer* l_temp = m_layers->find(name);
			if(l_temp!=NULL || name.compare(m_filename_drawing)==0)
			{
				if(m_active_frame)
				{
					hpaned1->remove(*m_active_frame);
					delete m_active_frame;
					m_active_frame=NULL;
				}
				if(m_con_layer_properties_data_changed)
				{
					m_con_layer_properties_data_changed.disconnect();
				}
				//get layers
				lLayerList layer_list;
				if(name.compare(m_filename_drawing)==0)
				{
					layer_list=m_layers->getLayerList();
				}
				else
				{
					layer_list.push_back(l_temp);
				}
				// add new drawing area
				Point2D p_lowleft;
				Point2D p_upright;
				m_layers->getMaxClippingArea(p_lowleft, p_upright);
				m_active_frame=Gtk::manage(new CadDrawingArea(layer_list, p_lowleft, p_upright));
				hpaned1->pack2(*m_active_frame, Gtk::EXPAND);
				m_active_frame->show();
			}
		}
	}	
}

void WindowMain::on_tree_layerprops_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
	Glib::RefPtr<Gtk::TreeSelection> selection = tree_layerprops->get_selection();
	Gtk::TreeModel::iterator it=selection->get_selected();
	button_tool_del->set_sensitive(false);
	button_layerprop_del->set_sensitive(false);
	if(it)
	{
		Gtk::TreeModel::Row row = *it;
		if(row[m_ColumnsTool.m_col_selectable])		
		{	
			Glib::ustring name=row[m_ColumnsLayerProps.m_col_name];
			LayerProperty* lp_temp = m_layerprops->find(name);
			if(lp_temp!=NULL)
			{
				if(m_active_frame)
				{
					hpaned1->remove(*m_active_frame);
					delete m_active_frame;
					m_active_frame=NULL;
				}
				if(m_con_layer_properties_data_changed)
				{
					m_con_layer_properties_data_changed.disconnect();
				}
				FrameLayerProperty* layer_properties=new class FrameLayerProperty(getGmmData(), *lp_temp, m_tools);
				m_active_frame = Gtk::manage(layer_properties);
				m_con_layer_properties_data_changed=layer_properties->signal_data_changed()
					.connect(sigc::mem_fun(*this, &WindowMain::on_layer_properties_data_changed));
				hpaned1->pack2(*m_active_frame, Gtk::EXPAND);
				
				m_active_frame->show();
				button_layerprop_del->set_sensitive(true);
			}
		}
	}	
}
		
void WindowMain::on_tree_tools_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{
	Glib::RefPtr<Gtk::TreeSelection> selection = tree_tools->get_selection();
	Gtk::TreeModel::iterator it=selection->get_selected();
	button_tool_del->set_sensitive(false);
	button_layerprop_del->set_sensitive(false);
	if(it)
	{
		Gtk::TreeModel::Row row = *it;
		int id = row[m_ColumnsTool.m_col_id];
		Tool* t_temp = m_tools->find(id);
		if(t_temp!=NULL)
		{
			if(m_active_frame)
			{
				hpaned1->remove(*m_active_frame);
				delete m_active_frame;
				m_active_frame=NULL;
			}
			if(m_con_layer_properties_data_changed)
			{
				m_con_layer_properties_data_changed.disconnect();
			}
			m_active_frame = Gtk::manage(new class FrameTool(getGmmData(), *t_temp, this));
			hpaned1->pack2(*m_active_frame, Gtk::EXPAND);
			m_active_frame->show();
			button_tool_del->set_sensitive(true);
		}
	}
	
}


void WindowMain::on_notebook1_switch_page(GtkNotebookPage *p0, guint p1)
{
	std::cout << "on_notebook1_switch_page: " << p1 << std::endl; 
	if(p1==PAD_TOOLS)
	{
		button_tool_add->set_sensitive(true);
	}
	else
	{
		button_tool_add->set_sensitive(false);
		//button_tool_del->set_sensitive(false);
	}
	if(p1==PAD_PROPERTIES)
	{
		button_layerprop_add->set_sensitive(true);
	}
	else
	{
		button_layerprop_add->set_sensitive(false);
		//button_layerprop_del->set_sensitive(false);
	}
	if(p1==PAD_DRAWING)
	{
		button_layerprop_add->set_sensitive(false);
		button_tool_add->set_sensitive(false);
		fillTreeLayers();
	}
	else
	{
		
	}

}

void WindowMain::on_button_layerprop_add_clicked()
{
	button_tool_del->set_sensitive(false);
	button_layerprop_del->set_sensitive(false);
	if(m_active_frame)
	{
		hpaned1->remove(*m_active_frame);
		delete m_active_frame;
		m_active_frame=NULL;
	}
	if(m_con_layer_properties_data_changed)
	{
		m_con_layer_properties_data_changed.disconnect();
	}
	LayerProperty lp_temp(m_tools);
	FrameLayerProperty* layer_properties=new class FrameLayerProperty(getGmmData(), lp_temp , m_tools);
	m_active_frame = Gtk::manage(layer_properties);
	m_con_layer_properties_data_changed=layer_properties->signal_data_changed()
		.connect(sigc::mem_fun(*this, &WindowMain::on_layer_properties_data_changed));
	hpaned1->pack2(*m_active_frame, Gtk::EXPAND);
	m_active_frame->show();
}

void WindowMain::on_button_layerprop_del_clicked()
{
	Glib::RefPtr<Gtk::TreeSelection> selection = tree_layerprops->get_selection();
	Gtk::TreeModel::iterator it=selection->get_selected();
	if(it)
	{
		Gtk::TreeModel::Row row = *it;
		Glib::ustring name = row[m_ColumnsTool.m_col_name];
		LayerProperty* lp_temp = m_layerprops->find(name);
		if(lp_temp!=NULL)
		{
			if(m_active_frame)
			{
				hpaned1->remove(*m_active_frame);
				delete m_active_frame;
				m_active_frame=NULL;
			}
			if(m_con_layer_properties_data_changed)
			{
				m_con_layer_properties_data_changed.disconnect();
			}
			m_layerprops->remove(name);
			m_active_frame = Gtk::manage(new class Gtk::Frame());
			hpaned1->pack2(*m_active_frame, Gtk::EXPAND);
			m_active_frame->show();
			fillTreeLayerProps();
			reloadLayers();
		}
	}
}

void WindowMain::on_button_tool_add_clicked()
{
	button_tool_del->set_sensitive(false);
	button_layerprop_del->set_sensitive(false);
	if(m_active_frame)
	{
		hpaned1->remove(*m_active_frame);
		delete m_active_frame;
		m_active_frame=NULL;
	}
	if(m_con_layer_properties_data_changed)
	{
		m_con_layer_properties_data_changed.disconnect();
	}
	Tool t_temp;
	m_active_frame = Gtk::manage(new class FrameTool(getGmmData(), t_temp, this));
	hpaned1->pack2(*m_active_frame, Gtk::EXPAND);
	m_active_frame->show();
}

void WindowMain::on_button_tool_del_clicked()
{
	//get selcted tool
	Glib::RefPtr<Gtk::TreeSelection> selection = tree_tools->get_selection();
	Gtk::TreeModel::iterator it=selection->get_selected();
	if(it)
	{
		Gtk::TreeModel::Row row = *it;
		int id = row[m_ColumnsTool.m_col_id];
		Tool* t_temp = m_tools->find(id);
		if(t_temp!=NULL)
		{
			m_tools->remove(id);
			if(m_active_frame)
			{
				hpaned1->remove(*m_active_frame);
				delete m_active_frame;
				m_active_frame=NULL;
			}
			if(m_con_layer_properties_data_changed)
			{
				m_con_layer_properties_data_changed.disconnect();
			}
			m_active_frame = Gtk::manage(new class Gtk::Frame());
			hpaned1->pack2(*m_active_frame, Gtk::EXPAND);
			m_active_frame->show();
			fillTreeTools();
			fillTreeLayerProps();
			reloadLayers();
		}
	}
	
}	

void WindowMain::loadConfig()
{
	std::string config_folder(g_get_home_dir());
	config_folder.append(1, G_DIR_SEPARATOR);
	config_folder.append(CONFIG_FOLDER);
	//
	std::string config_file(config_folder);
	config_file.append(1, G_DIR_SEPARATOR);
	config_file.append(CONFIG_FILE);
	//
	std::ifstream st_conf;
	st_conf.open(config_file.c_str());
	if(st_conf)
	{
		st_conf>>m_config_settings;
		st_conf.close();
	}
	// load default template
	std::string templ_file(config_folder);
	templ_file.append(1, G_DIR_SEPARATOR);
	templ_file.append(DEFAULT_TEMPLATE_FILE);
	//
	std::ifstream st_templ;
	st_templ.open(templ_file.c_str());
	if(st_templ)
	{
		loadTemplate(st_templ);
	}

}

void WindowMain::saveConfig()
{
	std::string config_folder(g_get_home_dir());
	config_folder.append(1, G_DIR_SEPARATOR);
	config_folder.append(CONFIG_FOLDER);
	// create directory if not created yet
	if(!g_file_test(config_folder.c_str(), G_FILE_TEST_EXISTS))
	{
		g_mkdir_with_parents(config_folder.c_str(), 0700);
	}
	
	//
	std::string config_file(config_folder);
	config_file.append(1, G_DIR_SEPARATOR);
	config_file.append(CONFIG_FILE);
	//
	std::ofstream output_stream;
	output_stream.open(config_file.c_str());
	if(!output_stream)
	{
		std::cout << "saveConfig:File not found!" << std::endl;
		throw(std::runtime_error("File not found!"));
		
	}
	output_stream << m_config_settings;
	output_stream << std::flush;
	output_stream.close();
	
}

void WindowMain::loadTemplate(std::istream &is)
{
	std::string line;
	std::getline(is, line);
	if(line.compare("[TOOLS]")==0)
	{
		std::stringstream st_tools;
		while(line.compare("[LAYERPROPS]")!=0 && !is.eof())
		{
			std::getline(is, line);
			st_tools<< line << std::endl;
		};
		st_tools >> *m_tools;
	}
	if(line.compare("[LAYERPROPS]")==0)
	{
		std::stringstream st_layerprops;
		while(!is.eof())
		{
			std::getline(is, line);
			st_layerprops<< line << std::endl;
		};
		st_layerprops >> *m_layerprops;
	}
}

void WindowMain::saveTemplate(std::ostream &os)
{
	os << "[TOOLS]" << std::endl;
	os << *m_tools;
	os << "[LAYERPROPS]" << std::endl;
	os << *m_layerprops;
}

void WindowMain::fillTreeTools()
{
	bool was_created_before=true;
	if(!m_refTreeModelTool)
	{
		m_refTreeModelTool = Gtk::ListStore::create(m_ColumnsTool);
		was_created_before=false;
	}
	m_refTreeModelTool->clear();
		
	tree_tools->set_model(m_refTreeModelTool);
	Gtk::TreeModel::Row row;
   	for(cItToolMap it=m_tools->getToolMap().begin(); it!=m_tools->getToolMap().end(); ++it)
	{
		Tool* t_temp = it->second;
		assert(t_temp);
		if(t_temp!=NULL)
		{
			row = *(m_refTreeModelTool->append());
			row[m_ColumnsTool.m_col_icon] = img_tool16;
			row[m_ColumnsTool.m_col_id] = t_temp->getNumber();
			//row[m_ColumnsTool.m_col_name] = t_temp->getComment();
		}
	}
	if(!was_created_before)
	{
		tree_tools->append_column("",m_ColumnsTool.m_col_icon);
		tree_tools->append_column("",m_ColumnsTool.m_col_id);
		tree_tools->set_headers_visible(false);
	}
	tree_tools->show_all_children();	

}

void WindowMain::fillTreeLayerProps()
{
	bool was_created_before=true;
	if(!m_refTreeModelLayerProps)
	{
		m_refTreeModelLayerProps = Gtk::TreeStore::create(m_ColumnsLayerProps);
		was_created_before=false;
	}
	m_refTreeModelLayerProps->clear();
		
	tree_layerprops->set_model(m_refTreeModelLayerProps);
	Gtk::TreeModel::Row row;
	////std::cout << "LayerProperties Count:" << m_layerprops->getLayerPropertyList().size()<< std::endl;
	for(cItLayerPropertyList it=m_layerprops->getLayerPropertyList().begin(); 
		it!=m_layerprops->getLayerPropertyList().end(); ++it)
	{
		LayerProperty* lp_temp = *it;
		assert(lp_temp);
		if(lp_temp!=NULL)
		{
			row = *(m_refTreeModelLayerProps->append());
			row[m_ColumnsTool.m_col_selectable] = true;	
			row[m_ColumnsTool.m_col_icon] = img_layerprop16;
			//row[m_ColumnsTool.m_col_id] = t_temp->getNumber();
			row[m_ColumnsTool.m_col_name] = lp_temp->getName();
			if(lp_temp->getTool())
			{
				// add child
				Gtk::TreeModel::Row child_row=*(m_refTreeModelLayerProps->append(row.children()));
				child_row[m_ColumnsTool.m_col_selectable] = false;
				child_row[m_ColumnsTool.m_col_icon] = img_tool16;
				std::stringstream id;
				id << lp_temp->getTool()->getNumber();
				child_row[m_ColumnsTool.m_col_name] = id.str();
			}
		}
	}
	if(!was_created_before)
	{
		tree_layerprops->append_column("",m_ColumnsTool.m_col_icon);
		tree_layerprops->append_column("",m_ColumnsTool.m_col_name);
		//tree_layerprops->append_column("",m_ColumnsTool.m_col_id);
		tree_layerprops->set_headers_visible(false);
	}
	tree_layerprops->show_all_children();		
}

void WindowMain::fillTreeLayers()
{
	bool was_created_before=true;
	if(!m_refTreeModelLayers)
	{
		m_refTreeModelLayers = Gtk::TreeStore::create(m_ColumnsLayers);
		was_created_before=false;
	}
	m_refTreeModelLayers->clear();
		
	tree_layers->set_model(m_refTreeModelLayers);
	//	
	if(m_layers)
	{
		Gtk::TreeModel::Row root_row= *(m_refTreeModelLayers->append());
		root_row[m_ColumnsLayers.m_col_selectable] = true;	
		root_row[m_ColumnsLayers.m_col_icon] = 	img_layer16;
		root_row[m_ColumnsLayers.m_col_name] = m_filename_drawing;
		std::cout << "filename: " << m_filename_drawing << std::endl;
		for(cItLayerList it=m_layers->getLayerList().begin(); it!=m_layers->getLayerList().end(); ++it)
		{
			Layer* l_temp = *it;
			assert(l_temp);
			if(l_temp!=NULL)
			{
				Gtk::TreeModel::Row layer_row = *(m_refTreeModelLayers->append(root_row.children()));
				layer_row[m_ColumnsLayers.m_col_selectable] = true;	
				layer_row[m_ColumnsLayers.m_col_icon] = img_layer16;
				layer_row[m_ColumnsLayers.m_col_name] = l_temp->getName();
				if(l_temp->getProperty()!=NULL)
				{
					// add LayerProperty 
					LayerProperty* lp_temp=l_temp->getProperty();
					assert(lp_temp);
					Gtk::TreeModel::Row property_row=*(m_refTreeModelLayers->append(layer_row.children()));
					property_row[m_ColumnsLayers.m_col_selectable] = false;
					property_row[m_ColumnsLayers.m_col_icon] = 	img_layerprop16;
					property_row[m_ColumnsLayers.m_col_name] = std::string(lp_temp->getName());
					if(lp_temp->getTool()!=NULL)
					{
						// add child
						Gtk::TreeModel::Row tool_row=*(m_refTreeModelLayers->append(property_row.children()));
						tool_row[m_ColumnsLayers.m_col_selectable] = false;
						tool_row[m_ColumnsLayers.m_col_icon] = 	img_tool16;
						std::stringstream id;
						id << lp_temp->getTool()->getNumber();
						tool_row[m_ColumnsLayers.m_col_name] = id.str();
					}
				}
			}
		}
	}
	if(!was_created_before)
	{
		tree_layers->append_column("",m_ColumnsLayers.m_col_icon);
		tree_layers->append_column("",m_ColumnsLayers.m_col_name);
		//tree_layerprops->append_column("",m_ColumnsTool.m_col_id);
		tree_layers->set_headers_visible(false);
	}
	tree_layers->show_all_children();		

}

bool WindowMain::on_layer_properties_data_changed(LayerProperty& old_lp, LayerProperty& new_lp)
{
	bool ret = true;
	if(m_layerprops->find(new_lp.getName()))
	{
		if((old_lp.getName()).compare(new_lp.getName())!=0)
		{
			Gtk::MessageDialog dlgError(*this, "Layer Name already used!",
										false, Gtk::MESSAGE_ERROR, 
										Gtk::BUTTONS_CANCEL);
			dlgError.run();
			ret=false;
		}
		else
		{
			m_layerprops->change(new_lp);
			reloadLayers();
			fillTreeLayerProps();
		}
	}
	else
	{
		if(m_layerprops->find(old_lp.getName()))
			m_layerprops->remove(old_lp.getName());
		m_layerprops->add(new_lp);
		reloadLayers();
		fillTreeLayerProps();
		
	}
	return ret;
}


bool WindowMain::on_tool_changed(Tool& old_tool, Tool& new_tool)
{
	bool ret =true;
	if(m_tools->find(new_tool.getNumber()))
	{
		if(old_tool.getNumber()!=new_tool.getNumber())
		{
			Gtk::MessageDialog dlgError(*this, "Tool Number already used!",
										false, Gtk::MESSAGE_ERROR, 
										Gtk::BUTTONS_CANCEL);
			dlgError.run();
			ret=false;
		}
		else
		{
			m_tools->change(new_tool);
		}
	}
	else
	{
		if(m_tools->find(old_tool.getNumber()))
			m_tools->remove(old_tool.getNumber());
		m_tools->add(new_tool);
		fillTreeTools();
		fillTreeLayerProps();
		reloadLayers();
	}
	return ret;
}
GlademmData *WindowMain::getGmmData()
{
	return NULL;
}

void WindowMain::reloadLayers()
{
	if(m_layers)
	{
		m_layers->reloadLayerProperties();
	}
	fillTreeLayers();
}

void WindowMain::loadImages()
{
	std::string path_app_icon = PACKAGE_PIXMAPS_DIR; 
	path_app_icon.append(1,G_DIR_SEPARATOR);
	path_app_icon.append("gcnccam.xpm");
	set_icon_from_file(path_app_icon);
	//
	std::string path_img_layer16 = PACKAGE_PIXMAPS_DIR; 
	path_img_layer16.append(1,G_DIR_SEPARATOR);
	path_img_layer16.append("layer16.png");
	img_layer16=Gdk::Pixbuf::create_from_file(path_img_layer16);
	//
	std::string path_img_layerprop16 = PACKAGE_PIXMAPS_DIR; 
	path_img_layerprop16.append(1,G_DIR_SEPARATOR);
	path_img_layerprop16.append("layerprop16.png");
	img_layerprop16=Gdk::Pixbuf::create_from_file(path_img_layerprop16);
	//
	std::string path_img_tool16 = PACKAGE_PIXMAPS_DIR; 
	path_img_tool16.append(1,G_DIR_SEPARATOR);
	path_img_tool16.append("tool16.png");
	img_tool16=Gdk::Pixbuf::create_from_file(path_img_tool16);
	//
	//
	//
	//
	
	std::string path_img_layerprop20 = PACKAGE_PIXMAPS_DIR; 
	path_img_layerprop20.append(1,G_DIR_SEPARATOR);
	path_img_layerprop20.append("layerprop20.png");
	Glib::RefPtr<Gdk::Pixbuf> buf_layerprop20=Gdk::Pixbuf::create_from_file(path_img_layerprop20);
	img_layerprop20 = Gtk::manage(new class Gtk::Image(buf_layerprop20));
	if(img_layerprop20)
	{
		button_layerprop_add->add(*img_layerprop20);
		img_layerprop20->show();
	}
	//
	std::string path_img_layerprop_del20 = PACKAGE_PIXMAPS_DIR; 
	path_img_layerprop_del20.append(1,G_DIR_SEPARATOR);
	path_img_layerprop_del20.append("layerprop_del20.png");
	Glib::RefPtr<Gdk::Pixbuf> buf_layerprop_del20=Gdk::Pixbuf::create_from_file(path_img_layerprop_del20);
	img_layerprop_del20 = Gtk::manage(new class Gtk::Image(buf_layerprop_del20));
	if(img_layerprop_del20)
	{
		button_layerprop_del->add(*img_layerprop_del20);
		img_layerprop_del20->show();
	}
	//
	std::string path_img_tool20 = PACKAGE_PIXMAPS_DIR; 
	path_img_tool20.append(1,G_DIR_SEPARATOR);
	path_img_tool20.append("layerprop20.png");
	Glib::RefPtr<Gdk::Pixbuf> buf_tool20=Gdk::Pixbuf::create_from_file(path_img_tool20);
	img_tool20 = Gtk::manage(new class Gtk::Image(buf_tool20));
	if(img_tool20)
	{
		button_tool_add->add(*img_tool20);
		img_tool20->show();
	}
	//
	//

	std::string path_img_tool_del20 = PACKAGE_PIXMAPS_DIR; 
	path_img_tool_del20.append(1,G_DIR_SEPARATOR);
	path_img_tool_del20.append("layerprop_del20.png");
	Glib::RefPtr<Gdk::Pixbuf> buf_tool_del20=Gdk::Pixbuf::create_from_file(path_img_tool_del20);
	img_tool_del20 = Gtk::manage(new class Gtk::Image(buf_tool_del20));
	if(img_tool_del20)
	{
		button_tool_del->add(*img_tool_del20);
		img_tool_del20->show();
	}
	
}
