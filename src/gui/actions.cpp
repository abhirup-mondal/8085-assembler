#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>

#include "../gui.hpp"
#include "../object.hpp"
#include "../debug.hpp"

int show_object_def = true;

void on_action_file_new() {

    auto buffer = p_source_editor->get_source_buffer();

    if (current_file == "") {
        std::string buffer_text = buffer->get_text();
        if (buffer_text != "") {
            on_action_save_as();
        }

        if (current_file != "") {
            buffer->set_text("");
        }
    }
    else {
        std::ifstream infile(current_file);
        std::string file_text;
        std::string line;

        while (std::getline(infile, line)) {
            file_text = file_text + line + '\n';
        }

        file_text = file_text + "";

        if (file_text != buffer->get_text()) {
            on_action_save_as();
        }

        file_text = "";
        while (std::getline(infile, line)) {
            file_text = file_text + line + '\n';
        }

        if (file_text == buffer->get_text()) {
            buffer->set_text("");
            current_file = "";
        }
    }
}

void on_action_save() {

    if (current_file != "") {
        std::ofstream outfile(current_file);

        auto buffer = p_source_editor->get_source_buffer();
        outfile << buffer->get_text();

        outfile.close();
    }
    else {
        on_action_save_as();
    }
}

void on_action_save_as() {
    Gtk::FileChooserDialog dialog("Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*p_main_window);

    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Save", Gtk::RESPONSE_OK);

    //Add filters, so that only certain file types can be selected:

    auto filter_asm = Gtk::FileFilter::create();
    filter_asm->set_name("ASM files");
    filter_asm->add_pattern("*.asm");
    filter_asm->add_mime_type("text/plain");
    filter_asm->add_mime_type("text/x-asm");
    dialog.add_filter(filter_asm);

    auto filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("C/C++ files");
    filter_cpp->add_mime_type("text/x-c");
    filter_cpp->add_mime_type("text/x-c++");
    filter_cpp->add_mime_type("text/x-c-header");
    dialog.add_filter(filter_cpp);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog.add_filter(filter_any);

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    //Handle the response:
    switch(result) {
      case (Gtk::RESPONSE_OK):
      {
        std::cout << "Save clicked." << std::endl;
        std::string filename = dialog.get_filename();
        std::cout << "File selected: " <<  filename << std::endl;

        current_file = filename;
        std::cout << current_file << std::endl;

        std::ofstream outfile(filename);

        auto buffer = p_source_editor->get_source_buffer();
        outfile << buffer->get_text();

        outfile.close();

        break;
      }

      case (Gtk::RESPONSE_CANCEL):
      {
        std::cout << "Cancel clicked." << std::endl;
        break;
      }

      default:
      {
        std::cout << "Unexpected button clicked." << std::endl;
        break;
      }
  }
}

void on_action_file_open() {
    Gtk::FileChooserDialog dialog("Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*p_main_window);

    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    //Add filters, so that only certain file types can be selected:

    auto filter_asm = Gtk::FileFilter::create();
    filter_asm->set_name("ASM files");
    filter_asm->add_pattern("*.asm");
    filter_asm->add_mime_type("text/plain");
    filter_asm->add_mime_type("text/x-asm");
    dialog.add_filter(filter_asm);

    auto filter_cpp = Gtk::FileFilter::create();
    filter_cpp->set_name("C/C++ files");
    filter_cpp->add_mime_type("text/x-c");
    filter_cpp->add_mime_type("text/x-c++");
    filter_cpp->add_mime_type("text/x-c-header");
    dialog.add_filter(filter_cpp);

    auto filter_any = Gtk::FileFilter::create();
    filter_any->set_name("Any files");
    filter_any->add_pattern("*");
    dialog.add_filter(filter_any);

    //Show the dialog and wait for a user response:
    int result = dialog.run();

    //Handle the response:
    switch(result) {
      case (Gtk::RESPONSE_OK):
      {
        std::cout << "Open clicked." << std::endl;
        std::string filename = dialog.get_filename();
        std::cout << "File selected: " <<  filename << std::endl;

        current_file = filename;
        std::cout << "Current File: " << current_file << std::endl;

        auto buffer = p_source_editor->get_source_buffer();
        buffer->set_text("");

        std::ifstream infile(filename);
        std::string line;

        while (std::getline(infile, line)) {
            line = line + "\n";
            buffer->insert(buffer->end(), line);
        }

        break;
      }

      case (Gtk::RESPONSE_CANCEL):
      {
        std::cout << "Cancel clicked." << std::endl;
        break;
      }

      default:
      {
        std::cout << "Unexpected button clicked." << std::endl;
        break;
      }
  }
}


void on_action_assemble() {
    std::cout << "File check to be done" << std::endl;

    auto buffer = p_source_editor->get_source_buffer();

    if (current_file == "") {
        std::string buffer_text = buffer->get_text();
        if (buffer_text != "") {
            on_action_save_as();
        }

        if (current_file == "") {
            return;
        }
    }
    else {
        std::ifstream infile(current_file);
        std::string file_text = "";
        std::string line;

        while (std::getline(infile, line)) {
            file_text = file_text + line + '\n';
        }

        file_text = file_text + "";

        bool file_new = false;

        if (file_text != buffer->get_text()) {
            file_new = true;
            on_action_save_as();
        }
        else {
            std::cout << "File is OK" << std::endl;
        }

        infile.close();

        if (file_new) {
            infile.open(current_file);

            std::string new_file_text = "";
            while (std::getline(infile, line)) {
                new_file_text = new_file_text + line + '\n';
            }

            new_file_text = new_file_text + "";

            if (new_file_text != buffer->get_text()) {
                std::cout << "File is NOT OK" << std::endl;
                return;
            }
        }
    }

    std::cout << "File check done" << std::endl;

    local_simulator->reset_simulator();

    auto rows = m_memory_treeview->get_model()->children();

    for (auto it = rows.begin(); it != rows.end(); ++it) {
        std::string s_address = (*it).get_value(mem_memory_col.m_col_memory);
        int address = hex_to_int(s_address);
        int value = (*it).get_value(mem_memory_col.m_col_value);
        local_simulator->set_memory(address,value);
    }

    std::cout << "Assembling File: " << current_file << std::endl;
    local_simulator->prepare_simulator(current_file);

    update_window();

    if (show_object_def) show_object(0, nullptr, local_simulator);
}

void on_action_debug() {
    show_object_def = false;
    on_action_assemble();
    show_object_def = true;

    show_debug(0, nullptr, local_simulator);
    update_debug();
}

void on_action_next() {
    local_simulator->execute_instruction();

    auto buffer = p_source_editor->get_buffer();
    Gtk::TextIter match_start = buffer->get_iter_at_line(local_simulator->registers["PC"]);
    Gtk::TextIter match_end   = buffer->get_iter_at_line(local_simulator->registers["PC"] + 1);

    buffer->select_range(match_start, match_end);
    p_source_editor->scroll_to(match_start);


    update_debug();
    update_window();
}

void on_action_execute() {

    std::cout << "File check to be done" << std::endl;

    auto buffer = p_source_editor->get_source_buffer();

    if (current_file == "") {
        std::string buffer_text = buffer->get_text();
        if (buffer_text != "") {
            on_action_save_as();
        }

        if (current_file == "") {
            return;
        }
    }
    else {
        std::ifstream infile(current_file);
        std::string file_text = "";
        std::string line;

        while (std::getline(infile, line)) {
            file_text = file_text + line + '\n';
        }

        file_text = file_text + "";

        bool file_new = false;

        if (file_text != buffer->get_text()) {
            file_new = true;
            on_action_save_as();
        }
        else {
            std::cout << "File is OK" << std::endl;
        }

        infile.close();

        if (file_new) {
            infile.open(current_file);

            std::string new_file_text = "";
            while (std::getline(infile, line)) {
                new_file_text = new_file_text + line + '\n';
            }

            new_file_text = new_file_text + "";

            if (new_file_text != buffer->get_text()) {
                std::cout << "File is NOT OK" << std::endl;
                return;
            }
        }
    }

    std::cout << "File check done" << std::endl;

    local_simulator->reset_simulator();

    auto rows = m_memory_treeview->get_model()->children();

    for (auto it = rows.begin(); it != rows.end(); ++it) {
        std::string s_address = (*it).get_value(mem_memory_col.m_col_memory);
        int address = hex_to_int(s_address);
        int value = (*it).get_value(mem_memory_col.m_col_value);
        local_simulator->set_memory(address,value);
    }

    std::cout << "Simulating File: " << current_file << std::endl;
    local_simulator->simulate(current_file);

    update_window();
}

void on_action_to_hex() {
    auto s = p_dec_entry->get_text();
    try {
        auto res = to_hex_string(std::stoi(s));
        p_hex_entry->set_text(res);
    }
    catch (std::exception& e) {
        std::cout << "Error" << std::endl;
        p_hex_entry->set_text("Error");
    }
}

void on_action_to_dec() {
    auto s = p_hex_entry->get_text();
    try {
        auto res = hex_to_int(s);
        p_dec_entry->set_text(std::to_string(res));
    }
    catch (std::exception& e) {
        std::cout << "Error" << std::endl;
        p_dec_entry->set_text("Error");
    }
}

void on_action_symtab() {
	show_symtab(0,nullptr, local_simulator);
}
