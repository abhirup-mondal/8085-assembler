#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>

#include "../gui.hpp"
#include "../object.hpp"

void create_memory_tree_view(Glib::RefPtr<Gtk::Builder> ref_builder,
                             Simulator* sim) {


    ref_builder->get_widget("memory-scroll-window", p_memory_scroll_window);
    p_memory_scroll_window->set_policy(
                             Gtk::POLICY_AUTOMATIC,
                             Gtk::POLICY_AUTOMATIC
                         );

    auto m_ref_memory_tree_model = Gtk::ListStore::create(mem_memory_col);

    ref_builder->get_widget("memory-view", m_memory_treeview);
    m_memory_treeview->set_model(m_ref_memory_tree_model);

    auto memory_data = sim->get_memory();

    for (int i = 0; i < memory_data.size(); ++i) {
        auto row = *(m_ref_memory_tree_model->append());

        auto address = Glib::ustring(to_hex_string(i));
        while (address.size() < 4) {
            address = '0' + address;
        }
        row[mem_memory_col.m_col_memory] = address;
        row[mem_memory_col.m_col_value] = short(memory_data[i]);
    }

    m_memory_treeview->
    append_column("Address", mem_memory_col.m_col_memory);
    m_memory_treeview->
    append_column_numeric_editable("Value", mem_memory_col.m_col_value, "%10x");
}

void create_register_tree_view(Glib::RefPtr<Gtk::Builder> ref_builder,
                             Simulator* sim) {

    ref_builder->get_widget("register-scroll-window", p_register_scroll_window);
    p_register_scroll_window->set_policy(
                             Gtk::POLICY_AUTOMATIC,
                             Gtk::POLICY_AUTOMATIC
                         );

    auto m_ref_register_tree_model = Gtk::ListStore::create(mem_register_col);

    ref_builder->get_widget("register-view", m_register_treeview);
    m_register_treeview->set_model(m_ref_register_tree_model);

    auto register_data = sim->get_registers();

    for (auto i : register_data) {
        auto row = *(m_ref_register_tree_model->append());
        row[mem_register_col.m_col_memory] = "    " + i.first;
        row[mem_register_col.m_col_value] = i.second;
    }


    m_register_treeview->
        append_column("   Register   ", mem_register_col.m_col_memory);
    m_register_treeview->
        append_column_numeric("        Value", mem_register_col.m_col_value, "%10x");
}

void create_flag_tree_view(Glib::RefPtr<Gtk::Builder> ref_builder,
                             Simulator* sim) {

    ref_builder->get_widget("flag-scroll-window", p_flag_scroll_window);
    p_flag_scroll_window->set_policy(
                             Gtk::POLICY_AUTOMATIC,
                             Gtk::POLICY_AUTOMATIC
                         );

    auto m_ref_flag_tree_model = Gtk::ListStore::create(mem_flag_col);

    ref_builder->get_widget("flag-view", m_flag_treeview);
    m_flag_treeview->set_model(m_ref_flag_tree_model);

    auto row = *(m_ref_flag_tree_model->append());
    row[mem_flag_col.m_col_memory] = "    AC";
    row[mem_flag_col.m_col_value] = 0;

    row = *(m_ref_flag_tree_model->append());
    row[mem_flag_col.m_col_memory] = "    C";
    row[mem_flag_col.m_col_value] = 0;

    row = *(m_ref_flag_tree_model->append());
    row[mem_flag_col.m_col_memory] = "    P";
    row[mem_flag_col.m_col_value] = 0;

    row = *(m_ref_flag_tree_model->append());
    row[mem_flag_col.m_col_memory] = "    S";
    row[mem_flag_col.m_col_value] = 0;

    row = *(m_ref_flag_tree_model->append());
    row[mem_flag_col.m_col_memory] = "    Z";
    row[mem_flag_col.m_col_value] = 0;

    m_flag_treeview->
        append_column("   Flag   ", mem_flag_col.m_col_memory);
    m_flag_treeview->
        append_column_numeric("        Value", mem_flag_col.m_col_value, "%10x");
}

void create_hex_dec(Glib::RefPtr<Gtk::Builder> ref_builder) {
    ref_builder->get_widget("dec-entry", p_dec_entry);
    ref_builder->get_widget("hex-entry", p_hex_entry);

    ref_builder->get_widget("to-dec-button", p_dec_button);
    p_dec_button->signal_clicked().connect(sigc::ptr_fun(on_action_to_dec));

    ref_builder->get_widget("to-hex-button", p_hex_button);
    p_hex_button->signal_clicked().connect(sigc::ptr_fun(on_action_to_hex));
}

void create_menu(Glib::RefPtr<Gtk::Builder> ref_builder) {

    ref_builder->get_widget("new-file", p_new_file);
    p_new_file->signal_activate().connect(sigc::ptr_fun(on_action_file_new));

    ref_builder->get_widget("open-file", p_open_file);
    p_open_file->signal_activate().connect(sigc::ptr_fun(on_action_file_open));


    ref_builder->get_widget("save-file", p_save);
    p_save->signal_activate().connect(sigc::ptr_fun(on_action_save));


    ref_builder->get_widget("save-as", p_save_as);
    p_save_as->signal_activate().connect(sigc::ptr_fun(on_action_save_as));

    ref_builder->get_widget("sym-tab", p_symtab);
    p_symtab->signal_activate().connect(sigc::ptr_fun(on_action_symtab));



    ref_builder->get_widget("menu-build-run", p_execute);
    p_execute->signal_activate().connect(sigc::ptr_fun(on_action_execute));

    ref_builder->get_widget("Assemble", p_assemble);
    p_assemble->signal_activate().connect(sigc::ptr_fun(on_action_assemble));

    ref_builder->get_widget("Debug", p_debug);
    p_debug->signal_activate().connect(sigc::ptr_fun(on_action_debug));


    ref_builder->get_widget("Next", p_next);
    p_next->signal_activate().connect(sigc::ptr_fun(on_action_next));
}

void create_toolbar(Glib::RefPtr<Gtk::Application> app, Glib::RefPtr<Gtk::Builder> ref_builder) {

	ref_builder->get_widget("new_button", p_new_button);
    app->add_action("new", sigc::ptr_fun(on_action_file_new));
    p_new_button->activate();

    ref_builder->get_widget("open_button", p_open_button);
    app->add_action("open", sigc::ptr_fun(on_action_file_open));
    p_open_button->activate();


    ref_builder->get_widget("save_button", p_save_button);
    app->add_action("save", sigc::ptr_fun(on_action_save));
    p_save_button->activate();
}


void create_terminal(Glib::RefPtr<Gtk::Builder> ref_builder,
                     Simulator* sim) {

    Gsv::init();

    ref_builder->get_widget("terminal-scroll-window", p_terminal_scroll_window);
    p_terminal_scroll_window->set_policy(
                          Gtk::POLICY_AUTOMATIC,
                          Gtk::POLICY_AUTOMATIC
                      );


    auto source_buffer = Gsv::Buffer::create();

    source_buffer->set_highlight_syntax(true);

    auto ssm = Gsv::StyleSchemeManager::create();
    auto paths = ssm->get_search_path();
    paths.push_back("./data");
    ssm->set_search_path(paths);
    auto scheme = ssm->get_scheme("jucse_1");
    source_buffer->set_style_scheme(scheme);

    auto lm = Gsv::LanguageManager::create();
    paths = lm->get_search_path();
    paths.push_back("./data");
    lm->set_search_path(paths);
    auto lang = lm->get_language("error_asm");

    source_buffer->set_language(lang);



    m_terminal_view = new Gsv::View(source_buffer);
    m_terminal_view->set_size_request(1200, 120);
    m_terminal_view->set_editable(false);
    m_terminal_view->set_visible(true);

    m_terminal_view->set_tab_width(4);
    m_terminal_view->set_auto_indent(true);
    m_terminal_view->set_insert_spaces_instead_of_tabs(true);
    m_terminal_view->set_highlight_current_line(true);
    m_terminal_view->set_show_line_marks(true);
    m_terminal_view->set_indent_on_tab(true);

    m_terminal_view->set_monospace(true);

    Glib::ustring font = "Fira Mono 12";
    auto font_description = new Pango::FontDescription(font);
    m_terminal_view->override_font(*font_description);


    p_terminal_scroll_window->add(*m_terminal_view);
}

void create_text_editor(Glib::RefPtr<Gtk::Builder> ref_builder,
                        Simulator* sim) {

    Gsv::init();
    ref_builder->get_widget("test-scroll-window", p_sourceview_window);
    p_sourceview_window->set_policy(
                             Gtk::POLICY_AUTOMATIC,
                             Gtk::POLICY_AUTOMATIC
                         );

    auto source_buffer = Gsv::Buffer::create();

    source_buffer->set_highlight_syntax(true);

    auto ssm = Gsv::StyleSchemeManager::create();
    auto paths = ssm->get_search_path();
    paths.push_back("./data");
    ssm->set_search_path(paths);
    auto scheme = ssm->get_scheme("jucse_1");
    source_buffer->set_style_scheme(scheme);

    auto lm = Gsv::LanguageManager::create();
    paths = lm->get_search_path();
    paths.push_back("./data");
    lm->set_search_path(paths);
    auto lang = lm->get_language("8085_asm");

    source_buffer->set_language(lang);



    p_source_editor = new Gsv::View(source_buffer);
    p_source_editor->set_size_request(800, 670);
    p_source_editor->set_editable(true);
    p_source_editor->set_visible(true);

    p_source_editor->set_show_line_numbers(true);
    p_source_editor->set_tab_width(8);
    p_source_editor->set_auto_indent(true);
    p_source_editor->set_insert_spaces_instead_of_tabs(true);
    p_source_editor->set_highlight_current_line(true);
    p_source_editor->set_show_line_marks(true);
    p_source_editor->set_indent_on_tab(true);
    p_source_editor->set_monospace(true);

    Glib::ustring font = "Fira Mono 14";
    auto font_description = new Pango::FontDescription(font);
    p_source_editor->override_font(*font_description);


    p_sourceview_window->add(*p_source_editor);


}
