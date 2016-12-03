#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>

#include "gui.hpp"
#include "object.hpp"

Glib::RefPtr<Gtk::Builder>  ref_builder; /* Look into this */
Gtk::ApplicationWindow*     p_main_window;

Gtk::ScrolledWindow*        p_sourceview_window = nullptr;
Gsv::View*                  p_source_editor = nullptr;

Gtk::ScrolledWindow*        p_memory_scroll_window = nullptr;
Gtk::TreeView*              m_memory_treeview      = nullptr;

Gtk::ScrolledWindow*        p_register_scroll_window = nullptr;
Gtk::TreeView*              m_register_treeview      = nullptr;

Gtk::ScrolledWindow*        p_flag_scroll_window = nullptr;
Gtk::TreeView*              m_flag_treeview      = nullptr;

Gtk::ScrolledWindow*        p_terminal_scroll_window = nullptr;
Gsv::View*                  m_terminal_view      = nullptr;

Gtk::Entry*                 p_hex_entry = nullptr;
Gtk::Entry*                 p_dec_entry = nullptr;
Gtk::Button*                p_hex_button = nullptr;
Gtk::Button*                p_dec_button = nullptr;

Gtk::MenuItem*              p_new_file  = nullptr;
Gtk::MenuItem*              p_open_file = nullptr;
Gtk::MenuItem*              p_save      = nullptr;
Gtk::MenuItem*              p_save_as   = nullptr;
Gtk::MenuItem*              p_execute   = nullptr;
Gtk::MenuItem*              p_assemble  = nullptr;
Gtk::MenuItem*              p_debug  = nullptr;

Gtk::MenuItem*              p_next = nullptr;
Gtk::MenuItem*			    p_symtab = nullptr;

Gtk::ToolButton*            p_open_button = nullptr;
Gtk::ToolButton*            p_save_button = nullptr;
Gtk::ToolButton*            p_new_button = nullptr;

Simulator* local_simulator;

std::string current_file = "";

Memory_Col mem_memory_col;
Memory_Col mem_register_col;
Memory_Col mem_flag_col;

int run_gui(int argc, char** argv, Simulator* sim) {

    local_simulator = sim;

    auto app = Gtk::Application::create(argc, argv, "assembler.jucse.gr-one");
    ref_builder = Gtk::Builder::create();

    load_from_glade(ref_builder);

    p_main_window = nullptr;
    ref_builder->get_widget("main-window", p_main_window);
    p_main_window->set_title("8085 Assembler and Simulator");

    create_memory_tree_view(ref_builder, sim);
    create_register_tree_view(ref_builder, sim);
    create_flag_tree_view(ref_builder, sim);
    create_menu(ref_builder);
    create_text_editor(ref_builder, sim);
    create_terminal(ref_builder, sim);
    create_toolbar(app, ref_builder);
    create_hex_dec(ref_builder);

    app->run(*p_main_window);

    delete p_new_file;
    delete p_open_file;
    delete p_save;
    delete p_save_as;
    delete p_execute;

    delete m_memory_treeview;
    delete p_memory_scroll_window;

    delete m_register_treeview;
    delete p_register_scroll_window;

    delete p_source_editor;
    delete p_main_window;

    return 0;
}

void load_from_glade(Glib::RefPtr<Gtk::Builder> ref_builder) {
    try {
      ref_builder->add_from_file("data/assembler-gui.glade");
    }
    catch(const Glib::FileError& ex) {
      std::cerr << "FileError: " << ex.what() << std::endl;
    }
    catch(const Glib::MarkupError& ex) {
      std::cerr << "MarkupError: " << ex.what() << std::endl;
    }
    catch(const Gtk::BuilderError& ex) {
      std::cerr << "BuilderError: " << ex.what() << std::endl;
    }
}

void update_window() {

    auto memory_rows = m_memory_treeview->get_model()->children();

    auto memory_data = local_simulator->get_memory();
    int i = 0;
    for (auto it = memory_rows.begin(); it != memory_rows.end(); ++it) {
        (*it)[mem_memory_col.m_col_value] = memory_data[i++];
    }

    auto register_data = local_simulator->get_registers();
    std::vector<int> temp_data;
    for (auto i : register_data) {
        temp_data.push_back(i.second);
    }



    auto register_rows = m_register_treeview->get_model()->children();
    i = 0;

    for (auto it = register_rows.begin(); it != register_rows.end(); ++it) {
        (*it)[mem_register_col.m_col_value] = temp_data[i++];
    }

    temp_data.clear();

    temp_data.push_back(local_simulator->AC);
    temp_data.push_back(local_simulator->C);
    temp_data.push_back(local_simulator->P);
    temp_data.push_back(local_simulator->S);
    temp_data.push_back(local_simulator->Z);

    auto flag_rows = m_flag_treeview->get_model()->children();
    i = 0;

    for (auto it = flag_rows.begin(); it != flag_rows.end(); ++it) {
        (*it)[mem_register_col.m_col_value] = temp_data[i++];
    }

    auto terminal_buffer = m_terminal_view->get_buffer();
    terminal_buffer->set_text("");

    for (auto i : local_simulator->assembler.error_data) {
        std::string error = local_simulator->get_error_string(i.second) + "on line " + std::to_string(i.first) + "\n";
        terminal_buffer->insert(terminal_buffer->end(), error);
    }
}
