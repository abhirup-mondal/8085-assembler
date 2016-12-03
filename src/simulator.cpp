#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "opcode.hpp"
#include "simulator.hpp"
#include "string_cleaner.hpp"

Simulator::Simulator(std::string opcode_data_file) {

    memory = std::vector<int>(MEMORY, 0);
    for (auto i : REGISTERS) {
        registers[i] = 0;
    }

    build_function_table();

    assembler = Assembler(opcode_data_file);
    assembling_completed = false;
    simulation_completed = false;
}

void Simulator::assemble(std::string asm_code_file) {
    reset_simulator();
    assembling_completed = false;
    assembler.assemble(asm_code_file);
    symbol_table = assembler.get_simulator_symbol_table();
    assembling_completed = true;
    simulation_completed = false;
}

void Simulator::reset_simulator() {
    //memory = std::vector<int>(MEMORY, 0);
    for (auto i : REGISTERS) {
        registers[i] = 0;
    }

    instruction_table.clear();
    symbol_table.clear();
    accessed_memory.clear();
    assembling_completed = false;

    AC = 0;
    C = 0;
    P = 0;
    S = 0;
    Z = 0;

    assembler.clean();
}

void Simulator::build_instruction_table(std::string asm_code_file) {
    std::ifstream code_file(asm_code_file);
    std::string   code_line;

    auto machine_code = assembler.get_machine_code();

    while (std::getline(code_file, code_line)) {

    	code_line = code_line.substr(0, code_line.find(';'));

        whitespace_clean(code_line);
        std::transform(
            code_line.begin(), code_line.end(),
            code_line.begin(), ::toupper
        );

        std::vector<std::string> tokens = split(code_line, ' ');
        instruction_table.push_back(tokens);
    }
}

std::string Simulator::get_error_string(int err) {
    switch (err) {
    case DATA_OUT_OF_RANGE_ERROR:
            return "ERROR: DATA OUT OF RANGE ";
            break;

    case ADDRESS_OUT_OF_RANGE_ERROR:
            return "ERROR: ADDRESS OUT OF RANGE ";
            break;

    case OPCODE_NOT_FOUND_ERROR:
            return "ERROR: OPCODE NOT FOUND ";
            break;

    case SYMBOL_NOT_FOUND_ERROR :
            return "ERROR: SYMBOL NOT FOUND ";
            break;

    case SYMBOL_REDEFINITION_ERROR:
            return "ERROR: SYMBOL REDEFINITION ";
            break;

    case ARITHMETIC_ERROR:
            return "ERROR: IN ARITHMETIC FUNCTION ";
            break;

    case MOVEMENT_ERROR:
            return "ERROR: BAD DATA MOVEMENT PARAMETERS ";
            break;

    case LOGICAL_ERROR:
            return "ERROR: IN LOGICAL FUNCTION ";
            break;

    case CONTROL_ERROR:
            return "ERROR: IN PROGRAM FLOW CONTROL FUNCTION ";
            break;

    case SYNTAX_ERROR:
            return "ERROR: INCORRECT SYNTAX ";
            break;

    default:
            return "FATAL ERROR: UNKNOWN ";
            break;
    }
}

void Simulator::build_function_table() {
    function_table["ACI"]   = &Simulator::aci;
    function_table["ADC"]   = &Simulator::adc;
    function_table["ADD"]   = &Simulator::add;
    function_table["ADI"]   = &Simulator::adi;
    function_table["ANA"]   = &Simulator::ana;
    function_table["ANI"]   = &Simulator::ani;
    function_table["CALL"]  = &Simulator::call;
    function_table["CC"]    = &Simulator::cc;
    function_table["CM"]    = &Simulator::cm;
    function_table["CMA"]   = &Simulator::cma;
    function_table["CMC"]   = &Simulator::cmc;
    function_table["CMP"]   = &Simulator::cmp;
    function_table["CNC"]   = &Simulator::cnc;
    function_table["CNZ"]   = &Simulator::cnz;
    function_table["CP"]    = &Simulator::cp;
    function_table["CPE"]   = &Simulator::cpe;
    function_table["CPI"]   = &Simulator::cpi;
    function_table["CPO"]   = &Simulator::cpo;
    function_table["CZ"]    = &Simulator::cz;
    function_table["DAA"]   = &Simulator::daa;
    function_table["DAD"]   = &Simulator::dad;
    function_table["DCR"]   = &Simulator::dcr;
    function_table["DCX"]   = &Simulator::dcx;
    function_table["DI"]    = &Simulator::di;
    function_table["EI"]    = &Simulator::ei;
    function_table["HLT"]   = &Simulator::hlt;
    function_table["IN"]    = &Simulator::in;
    function_table["INR"]   = &Simulator::inr;
    function_table["INX"]   = &Simulator::inx;
    function_table["JC"]    = &Simulator::jc;
    function_table["JM"]    = &Simulator::jm;
    function_table["JMP"]   = &Simulator::jmp;
    function_table["JNC"]   = &Simulator::jnc;
    function_table["JNZ"]   = &Simulator::jnz;
    function_table["JP"]    = &Simulator::jp;
    function_table["JPE"]   = &Simulator::jpe;
    function_table["JPO"]   = &Simulator::jpo;
    function_table["JZ"]    = &Simulator::jz;
    function_table["LDA"]   = &Simulator::lda;
    function_table["LDAX"]  = &Simulator::ldax;
    function_table["LHLD"]  = &Simulator::lhld;
    function_table["LXI"]   = &Simulator::lxi;
    function_table["MOV"]   = &Simulator::mov;
    function_table["MVI"]   = &Simulator::mvi;
    function_table["NOP"]   = &Simulator::nop;
    function_table["ORA"]   = &Simulator::ora;
    function_table["ORI"]   = &Simulator::ori;
    function_table["OUT"]   = &Simulator::out;
    function_table["PCHL"]  = &Simulator::pchl;
    function_table["POP"]   = &Simulator::pop;
    function_table["PUSH"]  = &Simulator::push;
    function_table["RAL"]   = &Simulator::ral;
    function_table["RAR"]   = &Simulator::rar;
    function_table["RC"]    = &Simulator::rc;
    function_table["RET"]   = &Simulator::ret;
    function_table["RIM"]   = &Simulator::rim;
    function_table["RLC"]   = &Simulator::rlc;
    function_table["RM"]    = &Simulator::rm;
    function_table["RNC"]   = &Simulator::rnc;
    function_table["RNZ"]   = &Simulator::rnz;
    function_table["RP"]    = &Simulator::rp;
    function_table["RPE"]   = &Simulator::rpe;
    function_table["RPO"]   = &Simulator::rpo;
    function_table["RRC"]   = &Simulator::rrc;
    function_table["RST"]   = &Simulator::rst;
    function_table["RZ"]    = &Simulator::rz;
    function_table["SBB"]   = &Simulator::sbb;
    function_table["SBI"]   = &Simulator::sbi;
    function_table["SHLD"]  = &Simulator::shld;
    function_table["SIM"]   = &Simulator::sim;
    function_table["SPHL"]  = &Simulator::sphl;
    function_table["STA"]   = &Simulator::sta;
    function_table["STAX"]  = &Simulator::stax;
    function_table["STC"]   = &Simulator::stc;
    function_table["SUB"]   = &Simulator::sub;
    function_table["SUI"]   = &Simulator::sui;
    function_table["XCHG"]  = &Simulator::xchg;
    function_table["XRA"]   = &Simulator::xra;
    function_table["XRI"]   = &Simulator::xri;
    function_table["XTHL"]  = &Simulator::xthl;

}

void Simulator::prepare_simulator(std::string asm_code_file) {
    reset_simulator();
    assembler.clean();
    assemble(asm_code_file);

    if (!assembling_completed) {
        std::cout << "Assembling failed. Unable to simulate." << std::endl;
        return;
    }
    else {
        std::cout << "Assembloing Complete " << std::endl;
    }

    build_instruction_table(asm_code_file);

    for (auto i : symbol_table) {
        std::cout << i.first << " " << i.second << std::endl;
    }

    registers["PC"] = 0;
}

void Simulator::simulate(std::string asm_code_file) {


    prepare_simulator(asm_code_file);

    while (!simulation_completed && !(registers["PC"] == instruction_table.size())) {
        if(!execute_instruction()) {
            assembler.error_data[registers["PC"]] = ERROR;
            simulator_error_data[registers["PC"]] = ERROR;

            for (auto i : simulator_error_data) {
                std::cout << i.first << " " << i.second << std::endl;
             }
            break;
        }
    }
}

int Simulator::execute_instruction() {
    std::vector<std::string> ins = instruction_table[registers["PC"]];
    if (ins.size()) {                   /* If not empty instruction */
        if (ins[0].back() == ':')  {    /* If first token is label */
            ins.erase(ins.begin());     /* Remove label */
        }
    }

    if (ins.size()) {
        if (function_table.count(ins[0])) {
            std::string function = ins[0]; /* Check if the function is valid */
            ins.erase(ins.begin());
            if(!(this->*function_table[function])(ins)) {
                return false;
            }
        }
    }
    else {
        ++registers["PC"]; /* Empty instruction. Go to next line. */
    }

    show_output();

    return true;
}

std::vector<int> Simulator::get_memory() {
    return memory;
}

std::map<std::string, int> Simulator::get_registers() {
    return registers;
}

int Simulator::set_memory(int address, int value) {
    if (0 <= address && address <= 0xFFFF) {
        memory[address] = value;
        return true;
    }
    else {
        return false;
    }
}

void Simulator::show_output() {
    for (auto i : registers) {
        std::cout << i.first << ": " << i.second << std::endl;
    }

    std::cout << std::endl;

    for (auto i : accessed_memory) {
        std::cout << to_hex_string(i) << "H: " << memory[i] << std::endl;
    }
}

void Simulator::set_flags(std::string r) {
    int value = registers[r];

    Z = (!value) ? true : false;
    S = (value & 0x80) ? true : false;

    int i=1;

    while (value) {
        if (value % 2) {
            i = !i;
        }
        value = value >> 2;
    }

    P = (i) ? true : false;
}

 /* ========================================================================= */

int Simulator::is_register(std::string args) {
     if(registers.count(args) && args.compare("PC") != 0)
         return true;
     //otherwise
     return false;
 }

int Simulator::parse_address(std::string address) {
    int res;
    if (address.back() == 'h' ||
        address.back() == 'H') {

        address.pop_back();
        res = hex_to_int(address);
    }
    else {
        std :: cout <<"padd\n";
        res = stoi(address);
    }

    if (0 <= res && res <= 0xFFFF) {
        accessed_memory.insert(res);
        return res;
    }
    else {
        std::cerr << "ERROR: Address out of range" << std::endl;
        return -1;
    }
}

int Simulator::parse_data(std::string data) {
    int res;
    if (data.back() == 'h' ||
        data.back() == 'H') {

        data.pop_back();
        res = hex_to_int(data);
    }
    else {
        std :: cout <<"pdata\n";
        res = stoi(data);
    }

    if (0 <= res && res <= 0xFF) {
        return res;
    }
    else {
        std::cerr << "ERROR: Data out of range" << std::endl;
        return -1;
    }

    return res;
}

int Simulator::parse_label(std::string label) {
    if (symbol_table.count(label)) {
        return symbol_table[label];
    }
    else {
        std::cerr << "ERROR: Undefined symbol";
        return -1;
    }
}
