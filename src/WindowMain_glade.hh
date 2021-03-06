// generated 2008/4/15 15:29:59 CEST by abegeman@sonyx.(none)
// using glademm V2.6.0
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/abegeman/projects/c_projekt/gcnccam/gcnccam.glade
// for gtk 2.8.20 and gtkmm 2.8.8
//
// Please modify the corresponding derived classes in ./src/WindowMain.hh and./src/WindowMain.cc

#ifndef _WINDOWMAIN_GLADE_HH
#  define _WINDOWMAIN_GLADE_HH

// Since all your widgets were private I made them all public.
// To differentiate between accessable (e.g. entries, output labels)
// and unaccessible widgets (e.g. static labels, containers)
// you should use the 'visibility' property (only visible in C++ mode)


#if !defined(GLADEMM_DATA)
#define GLADEMM_DATA 
#include <gtkmm/accelgroup.h>

class GlademmData
{  
        
        Glib::RefPtr<Gtk::AccelGroup> accgrp;
public:
        
        GlademmData(Glib::RefPtr<Gtk::AccelGroup> ag) : accgrp(ag)
        {  
        }
        
        Glib::RefPtr<Gtk::AccelGroup>  getAccelGroup()
        {  return accgrp;
        }
};
#endif //GLADEMM_DATA

#include <gtkmm/window.h>
#include <gtkmm/tooltips.h>
#include <gtkmm/menuitem.h>
#include <gtkmm/imagemenuitem.h>
#include <gtkmm/menu.h>
#include <gtkmm/menubar.h>
#include <gtkmm/image.h>
#include <gtkmm/button.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#include <gtkmm/toolitem.h>
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#endif //
#include <gtkmm/separator.h>
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#endif //
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
#endif //
#include <gtkmm/toolbar.h>
#include <gtkmm/treeview.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/label.h>
#include <gtkmm/notebook.h>
#include <gtkmm/alignment.h>
#include <gtkmm/frame.h>
#include <gtkmm/paned.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/box.h>

class WindowMain_glade : public Gtk::Window
{  
        
        GlademmData *gmm_data;
public:
        Gtk::Tooltips _tooltips;
        class Gtk::Window * WindowMain;
        class Gtk::MenuItem * open_template;
        class Gtk::MenuItem * save_template;
        class Gtk::MenuItem * separator2;
        class Gtk::MenuItem * open_drawing;
        class Gtk::MenuItem * separator3;
        class Gtk::MenuItem * save_cnc_file;
        class Gtk::MenuItem * separator4;
        class Gtk::MenuItem * export_tool_file1;
        class Gtk::MenuItem * separatormenuitem2;
        class Gtk::ImageMenuItem * quit1;
        class Gtk::Menu * menuitem_file_menu;
        class Gtk::MenuItem * menuitem_file;
        class Gtk::MenuItem * save_template_default;
        class Gtk::MenuItem * settings;
        class Gtk::Menu * menu_item_edit_menu;
        class Gtk::MenuItem * menu_item_edit;
        class Gtk::MenuItem * build_cnc_code;
        class Gtk::Menu * menuitem_build_menu;
        class Gtk::MenuItem * menuitem_build;
        class Gtk::ImageMenuItem * about;
        class Gtk::Menu * menuitem_help_menu;
        class Gtk::MenuItem * menuitem_help;
        class Gtk::MenuBar * menubar2;
        class Gtk::Image * image48;
        class Gtk::Button * button_open_drawing;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
        class Gtk::ToolItem * toolitem14;
#endif //
        class Gtk::Image * image30;
        class Gtk::Button * button_save_cnc_code;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
        class Gtk::ToolItem * toolitem2;
#endif //
        class Gtk::Image * image31;
        class Gtk::Button * button_build_cnc;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
        class Gtk::ToolItem * toolitem3;
#endif //
        class Gtk::VSeparator * vseparator1;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
        class Gtk::ToolItem * toolitem5;
#endif //
        class Gtk::VSeparator * vseparator2;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
        class Gtk::ToolItem * toolitem8;
#endif //
        class Gtk::Button * button_layerprop_add;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
        class Gtk::ToolItem * toolitem9;
#endif //
        class Gtk::Button * button_layerprop_del;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
        class Gtk::ToolItem * toolitem10;
#endif //
        class Gtk::VSeparator * vseparator3;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
        class Gtk::ToolItem * toolitem11;
#endif //
        class Gtk::Button * button_tool_add;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
        class Gtk::ToolItem * toolitem12;
#endif //
        class Gtk::Button * button_tool_del;
#if GTKMM_MAJOR_VERSION==2 && GTKMM_MINOR_VERSION>2
        class Gtk::ToolItem * toolitem15;
#endif //
        class Gtk::Toolbar * toolbar;
        class Gtk::TreeView * tree_layers;
        class Gtk::ScrolledWindow * scrolledwindow5;
        class Gtk::Label * label6;
        class Gtk::TreeView * tree_layerprops;
        class Gtk::ScrolledWindow * scrolledwindow6;
        class Gtk::Label * label7;
        class Gtk::TreeView * tree_tools;
        class Gtk::ScrolledWindow * scrolledwindow7;
        class Gtk::Label * label8;
        class Gtk::Notebook * notebook1;
        class Gtk::Label * label29;
        class Gtk::Alignment * alignment2;
        class Gtk::Frame * frame_default;
        class Gtk::HPaned * hpaned1;
        class Gtk::Statusbar * statusbar1;
        class Gtk::VBox * vbox1;
protected:
        
        WindowMain_glade();
        
        ~WindowMain_glade();
private:
        virtual void on_open_template_activate() = 0;
        virtual void on_save_template_activate() = 0;
        virtual void on_open_drawing_activate() = 0;
        virtual void on_save_cnc_file_activate() = 0;
        virtual void on_export_tool_file1_activate() = 0;
        virtual void on_quit1_activate() = 0;
        virtual void on_save_template_default_activate() = 0;
        virtual void on_settings_activate() = 0;
        virtual void on_build_cnc_code_activate() = 0;
        virtual void on_about_activate() = 0;
        virtual void on_button_open_drawing_clicked() = 0;
        virtual void on_button_save_cnc_code_clicked() = 0;
        virtual void on_button_build_cnc_clicked() = 0;
        virtual void on_button_layerprop_add_clicked() = 0;
        virtual void on_button_layerprop_del_clicked() = 0;
        virtual void on_button_tool_add_clicked() = 0;
        virtual void on_button_tool_del_clicked() = 0;
        virtual void on_tree_layers_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) = 0;
        virtual void on_tree_layerprops_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) = 0;
        virtual void on_tree_tools_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) = 0;
        virtual void on_notebook1_switch_page(GtkNotebookPage *p0, guint p1) = 0;
};
#endif
