#include <map>
#include <string>
#include <vector>

#include "assembler.hpp"
#include "gui.hpp"
#include "opcode.hpp"
#include "string_cleaner.hpp"

int main(int argc, char** argv) {

    /*
    Assembler asmb = Assembler();
    asmb.assemble("../data/test_code.asm");
    asmb.print_machine_code();
    */

    Simulator sim = Simulator();
    
    /*sim.set_memory(hex_to_int("3000H"), 12);
    sim.set_memory(hex_to_int("3001H"), 11);
    sim.simulate("../data/test_code.asm");
    sim.show_output();*/

    run_gui(argc, argv, &sim);

    return 0;
}
