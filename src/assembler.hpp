#ifndef _GUARD_ASSEMBLER_HPP
#define _GUARD_ASSEMBLER_HPP

#include <bitset>
#include <map>
#include <unordered_map>
#include <vector>

#include "opcode.hpp"

#define DATA_OUT_OF_RANGE_ERROR     -10
#define ADDRESS_OUT_OF_RANGE_ERROR  -11
#define OPCODE_NOT_FOUND_ERROR      -12
#define SYMBOL_NOT_FOUND_ERROR      -13
#define SYMBOL_REDEFINITION_ERROR   -14
#define ARITHMETIC_ERROR            -15
#define MOVEMENT_ERROR              -16
#define LOGICAL_ERROR               -17
#define CONTROL_ERROR               -18
#define SYNTAX_ERROR                -19
#define ERROR                       -20

class Assembler {
public:
    std::unordered_map<std::string, std::vector<Opcode>> opcode_table;
    std::unordered_map<std::string, int>                 symbol_table;
    std::unordered_map<std::string, int>                 simulator_symbol_table;

    std::vector<std::string> machine_code;



    bool pass_one_completed;

    void pass_one(std::string asm_code_file);
    void pass_two(std::string asm_code_file);

    std::map<int,int> error_data;

    Assembler(std::string opcode_data_file = "data/opcode_table.txt");

    void assemble(std::string asm_code_file);

    void print_opcode_table();
    void print_symbol_table();
    void print_machine_code();

    std::unordered_map<std::string, int> get_symbol_table();
    std::unordered_map<std::string, int> get_simulator_symbol_table();
    std::unordered_map<std::string, std::vector<Opcode>> get_opcode_table();
    std::vector<std::string> get_machine_code();
    std::map<int,int> get_error_data();


    void clean();
};

std::string to_hex_string(int i);
int hex_to_int(std::string hex);
std::string hex_to_bin(std::string hex);

#endif
