#ifndef _GUARD_OBJECT_HPP
#define _GUARD_OBJECT_HPP

#include <gtkmm.h>
#include <gtksourceviewmm.h>
#include <iostream>

#include "simulator.hpp"



int show_object(int argc, char** argv, Simulator* sim);
void object_load_from_glade(Glib::RefPtr<Gtk::Builder> ref_builder);
void close_object();

#endif
