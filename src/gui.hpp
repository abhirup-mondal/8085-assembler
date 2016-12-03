#ifndef _GUARD_GUI_HPP
#define _GUARD_GUI_HPP

#include <exception>
#include <gtkmm.h>
#include <gtksourceviewmm.h>
#include <gtksourceviewmm/language.h>
#include <iostream>
#include <stdexcept>

#include "string_cleaner.hpp"
#include "debug.hpp"
#include "object.hpp"
#include "simulator.hpp"
#include "symtab.hpp"

class Memory_Col : public Gtk::TreeModel::ColumnRecord {
public:
    Gtk::TreeModelColumn<Glib::ustring> m_col_memory;
    Gtk::TreeModelColumn<short> m_col_value;

    Memory_Col(){
        add(m_col_memory);
        add(m_col_value);
    }
};

int run_gui(int argc, char** argv, Simulator* sim);
void load_from_glade(Glib::RefPtr<Gtk::Builder> ref_builder);


void create_memory_tree_view(Glib::RefPtr<Gtk::Builder> ref_builder,
                             Simulator* sim);
void create_register_tree_view(Glib::RefPtr<Gtk::Builder> ref_builder,
                               Simulator* sim);
void create_flag_tree_view(Glib::RefPtr<Gtk::Builder> ref_builder,
                           Simulator* sim);

void create_text_editor(Glib::RefPtr<Gtk::Builder> ref_builder,
                        Simulator* sim);

void create_terminal(Glib::RefPtr<Gtk::Builder> ref_builder,
                     Simulator* sim);

void create_menu(Glib::RefPtr<Gtk::Builder> ref_builder);
void create_hex_dec(Glib::RefPtr<Gtk::Builder> ref_builder);

void create_toolbar(Glib::RefPtr<Gtk::Application> app, Glib::RefPtr<Gtk::Builder> ref_builder);

void on_action_file_new();
void on_action_file_open();
void on_action_save_as();
void on_action_save();

void on_action_assemble();
void on_action_execute();
void on_action_next();
void on_action_debug();

void on_action_to_dec();
void on_action_to_hex();

void on_action_symtab();

void update_window();

extern Glib::RefPtr<Gtk::Builder>  ref_builder;
extern Gtk::ApplicationWindow*     p_main_window;

extern Gtk::ScrolledWindow*        p_sourceview_window;
extern Gsv::View*                  p_source_editor;

extern Gtk::ScrolledWindow*        p_memory_scroll_window;
extern Gtk::TreeView*              m_memory_treeview;

extern Gtk::ScrolledWindow*        p_register_scroll_window;
extern Gtk::TreeView*              m_register_treeview;

extern Gtk::ScrolledWindow*        p_flag_scroll_window;
extern Gtk::TreeView*              m_flag_treeview;

extern Gtk::Entry*                 p_hex_entry;
extern Gtk::Entry*                 p_dec_entry;

extern Gtk::Button*                p_hex_button;
extern Gtk::Button*                p_dec_button;

extern Gtk::ScrolledWindow*        p_terminal_scroll_window;
extern Gsv::View*                  m_terminal_view;

extern Gtk::MenuItem*              p_new_file;
extern Gtk::MenuItem*              p_open_file;
extern Gtk::MenuItem*              p_save;
extern Gtk::MenuItem*              p_save_as;
extern Gtk::MenuItem*              p_execute;
extern Gtk::MenuItem*              p_assemble;
extern Gtk::MenuItem*              p_debug;

extern Gtk::MenuItem*			   p_symtab;


extern Gtk::MenuItem*              p_next;


extern Gtk::ToolButton*            p_open_button;
extern Gtk::ToolButton*            p_save_button;
extern Gtk::ToolButton*            p_new_button;


extern Simulator* local_simulator;

extern std::string current_file;

extern Memory_Col mem_memory_col;
extern Memory_Col mem_register_col;
extern Memory_Col mem_flag_col;

#endif
