#ifndef _GUARD_DEBUG_HPP
#define _GUARD_DEBUG_HPP

#include <gtkmm.h>
#include <iostream>
#include <string>
#include <vector>

#include <gtksourceviewmm.h>

#include "simulator.hpp"
#include "assembler.hpp"
#include "gui.hpp"


int show_debug(int argc, char** argv, Simulator* sim);
void debugger_load_from_glade(Glib::RefPtr<Gtk::Builder> ref_builder);

void on_close_debugger();
void update_debug();

#endif
