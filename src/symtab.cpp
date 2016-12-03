#include "symtab.hpp"


Glib::RefPtr<Gtk::Builder>  s_ref_builder;
Gtk::ApplicationWindow*     s_p_main_window;

Gtk::ScrolledWindow*        s_scroll_window = nullptr;
Gsv::View*                  s_p_text_view = nullptr;

Simulator* s_local_simulator;



int show_symtab(int argc, char** argv, Simulator* sim) {

	    std::cout << "Symtab" << std::endl;

    s_local_simulator = sim;

    auto app = Gtk::Application::create(argc, argv, "assembler.jucse.gr-one-symtab");
    s_ref_builder = Gtk::Builder::create();


    symtab_load_from_glade(s_ref_builder);


    s_p_main_window = nullptr;
    s_ref_builder->get_widget("main-window-object", s_p_main_window);
    s_p_main_window->set_title("Symbol Table");

    s_ref_builder->get_widget("scroll-window", s_scroll_window);
    s_scroll_window->set_policy(
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

    s_p_text_view = new Gsv::View(source_buffer);
    s_p_text_view->set_size_request(800, 670);
    s_p_text_view->set_editable(false);
    s_p_text_view->set_visible(true);

    s_p_text_view->set_show_line_numbers(false);
    s_p_text_view->set_tab_width(8);
    s_p_text_view->set_auto_indent(true);
    s_p_text_view->set_insert_spaces_instead_of_tabs(true);
    s_p_text_view->set_highlight_current_line(true);
    s_p_text_view->set_show_line_marks(true);
    s_p_text_view->set_indent_on_tab(true);
    s_p_text_view->set_monospace(true);

    Glib::ustring font = "Fira Mono 12";
    auto font_description = new Pango::FontDescription(font);
    s_p_text_view->override_font(*font_description);

    auto terminal_buffer = Gtk::TextBuffer::create();

    source_buffer->set_text("");

    for (auto i : s_local_simulator->assembler.symbol_table) {
        source_buffer->insert(source_buffer->end(), i.first + ":\t" + std::to_string(i.second) + "\n");
    }

    s_scroll_window->add(*s_p_text_view);

    std::cout << "Symtab" << std::endl;

    app->run(*s_p_main_window);

    delete s_p_text_view;
    delete s_p_main_window;


    return 0;
}

void close_symtab() {
    s_p_main_window->close();
}


void symtab_load_from_glade(Glib::RefPtr<Gtk::Builder> ref_builder) {
    try {
      s_ref_builder->add_from_file("data/symtab.glade");
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
