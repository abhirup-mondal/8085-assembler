#include "debug.hpp"


Glib::RefPtr<Gtk::Builder>  d_ref_builder;
Gtk::ApplicationWindow*     d_p_main_window;

Gtk::ScrolledWindow*        d_p_scroll_window = nullptr;
Gtk::TextView*              d_p_text_view = nullptr;

Gtk::Button*                d_p_next_button = nullptr;
Gtk::Button*                d_p_close_button = nullptr;

Simulator* d_local_simulator;

Gsv::View*                  d_p_debug_view;

int show_debug(int argc, char** argv, Simulator* sim) {

    d_local_simulator = sim;

    auto app = Gtk::Application::create(argc, argv, "assembler.jucse.gr-one-debug");
    d_ref_builder = Gtk::Builder::create();


    debugger_load_from_glade(d_ref_builder);


    d_p_main_window = nullptr;
    d_ref_builder->get_widget("main-window", d_p_main_window);
    d_p_main_window->set_title("Debug");

    d_ref_builder->get_widget("scrolledwindow1", d_p_scroll_window);
    d_p_scroll_window->set_policy(
                          Gtk::POLICY_AUTOMATIC,
                          Gtk::POLICY_AUTOMATIC
                      );

    Gsv::init();

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

    d_p_debug_view = new Gsv::View(source_buffer);
    d_p_debug_view->set_size_request(800, 670);
    d_p_debug_view->set_editable(true);
    d_p_debug_view->set_visible(true);

    d_p_debug_view->set_show_line_numbers(false);
    d_p_debug_view->set_tab_width(8);
    d_p_debug_view->set_auto_indent(true);
    d_p_debug_view->set_insert_spaces_instead_of_tabs(true);
    d_p_debug_view->set_highlight_current_line(true);
    d_p_debug_view->set_show_line_marks(true);
    d_p_debug_view->set_indent_on_tab(true);
    d_p_debug_view->set_monospace(true);

    Glib::ustring font = "Fira Mono 12";
    auto font_description = new Pango::FontDescription(font);
    d_p_debug_view->override_font(*font_description);


    d_p_scroll_window->add(*d_p_debug_view);

    d_ref_builder->get_widget("next-button", d_p_next_button);
    d_p_next_button->signal_clicked().connect(sigc::ptr_fun(on_action_next));

    d_ref_builder->get_widget("close-button", d_p_close_button);
    d_p_close_button->signal_clicked().connect(sigc::ptr_fun(on_close_debugger));


    app->run(*d_p_main_window);

    delete d_p_next_button;
    delete d_p_close_button;

    delete d_p_text_view;
    delete d_p_main_window;

    return 0;
}

void update_debug() {

    auto buffer = d_p_debug_view->get_buffer();
    buffer->set_text("");

    for (auto i : d_local_simulator->registers) {
        std::string s = i.first + ": " + std::to_string(i.second) + "\n";
        buffer->insert(buffer->end(), s);
    }

    buffer->insert(buffer->end(), "\n");

    for (auto i : d_local_simulator->accessed_memory) {
        std::string s = to_hex_string(i) + "H:\t" + std::to_string(d_local_simulator->memory[i]) + "\n";
        buffer->insert(buffer->end(), s);
    }
}

void on_close_debugger() {
    local_simulator->reset_simulator();
    d_p_main_window->close();
}

void debugger_load_from_glade(Glib::RefPtr<Gtk::Builder> ref_builder) {
    try {
      d_ref_builder->add_from_file("data/debugger.glade");
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
