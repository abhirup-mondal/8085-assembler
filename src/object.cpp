#include "object.hpp"


Glib::RefPtr<Gtk::Builder>  o_ref_builder;
Gtk::ApplicationWindow*     o_p_main_window;

Gtk::ScrolledWindow*        p_scroll_window = nullptr;
Gsv::View*                  o_p_text_view = nullptr;

Simulator* o_local_simulator;



int show_object(int argc, char** argv, Simulator* sim) {

    o_local_simulator = sim;

    auto app = Gtk::Application::create(argc, argv, "assembler.jucse.gr-one-object");
    o_ref_builder = Gtk::Builder::create();


    object_load_from_glade(o_ref_builder);


    o_p_main_window = nullptr;
    o_ref_builder->get_widget("main-window-object", o_p_main_window);
    o_p_main_window->set_title("Object Code");

    o_ref_builder->get_widget("scroll-window", p_scroll_window);
    p_scroll_window->set_policy(
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
    auto lang = lm->get_language("error_asm");

    source_buffer->set_language(lang);

    o_p_text_view = new Gsv::View(source_buffer);
    o_p_text_view->set_size_request(800, 670);
    o_p_text_view->set_editable(false);
    o_p_text_view->set_visible(true);

    o_p_text_view->set_show_line_numbers(true);
    o_p_text_view->set_tab_width(8);
    o_p_text_view->set_auto_indent(true);
    o_p_text_view->set_insert_spaces_instead_of_tabs(true);
    o_p_text_view->set_highlight_current_line(true);
    o_p_text_view->set_show_line_marks(true);
    o_p_text_view->set_indent_on_tab(true);
    o_p_text_view->set_monospace(true);

    Glib::ustring font = "Fira Mono 12";
    auto font_description = new Pango::FontDescription(font);
    o_p_text_view->override_font(*font_description);

    auto terminal_buffer = Gtk::TextBuffer::create();

    source_buffer->set_text("");

    std::string hex = "";
    std::string bin = "";

    for (auto i : o_local_simulator->assembler.machine_code) {
        if (i == "-") {

        	while (hex.size() < 14) {
        		hex += " ";
        	}

            source_buffer->insert(source_buffer->end(), hex + " ||  " + bin + "\n");
            hex = "";
            bin = "";
        }
		else {
			hex += " " + i;
			bin += " " + hex_to_bin(i);
		}
    }

    p_scroll_window->add(*o_p_text_view);

    app->run(*o_p_main_window);

    delete o_p_text_view;
    delete o_p_main_window;


    return 0;
}

void close_object() {
    o_p_main_window->close();
}


void object_load_from_glade(Glib::RefPtr<Gtk::Builder> ref_builder) {
    try {
      o_ref_builder->add_from_file("data/object-code.glade");
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
