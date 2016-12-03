#ifndef _GUARD_SYMTAB_HPP
#define _GUARD_SYMTAB_HPP

#include <gtkmm.h>
#include <gtksourceviewmm.h>
#include <iostream>

#include "simulator.hpp"



int show_symtab(int argc, char** argv, Simulator* sim);
void symtab_load_from_glade(Glib::RefPtr<Gtk::Builder> ref_builder);
void close_symtab();

#endif
