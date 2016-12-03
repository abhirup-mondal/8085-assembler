#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <bitset>

#include "assembler.hpp"
#include "string_cleaner.hpp"

std::string to_hex_string(int i) {
    std::stringstream sstream;
    sstream << std::hex << i;
    std::string result = sstream.str();
    if (result.size() == 1) {
        result = '0' + result;
    }

    std::transform(
        result.begin(), result.end(),
        result.begin(), ::toupper
    );

    return result;
}

std::string hex_to_bin(std::string hex) {

	hex = "0x" + hex;

    std::stringstream sstream;
    sstream << std::hex << hex;
    unsigned y;
    sstream >> y;
    std::bitset<8> b(y);
    return b.to_string();
}

int hex_to_int(std::string hex) {
    std::stringstream sstream;
    sstream << hex;
    int y;
    sstream >> std::hex >> y;
    return y;
}

Assembler::Assembler(std::string opcode_data_file) {
    opcode_table = build_opcode_table(opcode_data_file);
    pass_one_completed = false;
}

void Assembler::pass_one(std::string asm_code_file) {

    std::ifstream code_file(asm_code_file);
    std::string   code_line;
    int line_no = 1;
    int address = 0;

    while (std::getline(code_file, code_line)) {

    	code_line = code_line.substr(0, code_line.find(';'));

        whitespace_clean(code_line);
        std::transform(
            code_line.begin(), code_line.end(),
            code_line.begin(), ::toupper
        );

        std::cout << code_line << std::endl;

        std::vector<std::string> tokens = split(code_line, ' ');

        if (tokens.size()) {                    /* If not empty line */
            if (tokens[0].back() == ':')  {     /* If line is labelled */

                /* Remove ':' from the end of label */
                std::string symbol = tokens[0].substr(0, tokens[0].size() - 1);

                if (!symbol_table.count(symbol)) {  /* If symbol is undefined */
                    symbol_table[symbol] = address;
                    simulator_symbol_table[symbol] = line_no - 1;
                }
                else {
                    std::cout << "Error: Redifinition of symbol on line "
                              << line_no << "!" << std::endl;

                    error_data[line_no] = SYMBOL_REDEFINITION_ERROR;
                }

                tokens.erase(tokens.begin()); /* Remove label from tokens */
            }
        }

        if (tokens.size()) { /* If symbol is not in empty line */
            std::string opcode = tokens[0];
            if (opcode_table.count(opcode)) { /* If opcode is valid */
                /* Then increase address by byte count of opcode */
                address += opcode_table[opcode][0].get_byte_count();
            }
            else { /* invalid Instruction */
                std::cout << "Error: invalid operation \"" + opcode + "\"\n";
                error_data[line_no] = OPCODE_NOT_FOUND_ERROR;
                break;
            }
        }
        else {
            ++address;
        }

        ++line_no;
    }

    pass_one_completed = true;
}


void Assembler::pass_two(std::string asm_code_file) {

    if (!pass_one_completed) {
        std::cout << "Error: Pass 1 not completed yet" << std::endl;
    }

    std::ifstream code_file(asm_code_file);
    std::string   code_line;
    int line_no = 1;

    while (std::getline(code_file, code_line)) {

    	code_line = code_line.substr(0, code_line.find(';'));

        whitespace_clean(code_line);
        std::transform(
            code_line.begin(), code_line.end(),
            code_line.begin(), ::toupper
        );
        std::vector<std::string> tokens = split(code_line, ' ');

        if (tokens.size()) {                    /* If not empty line */
            if (tokens[0].back() == ':')  {     /* If line is labelled */
                tokens.erase(tokens.begin());
            }
        }

        if (tokens.size()) {
            std::string opcode = tokens[0];
            if (opcode_table.count(opcode)) {
                tokens.erase(tokens.begin());
                bool match_found = false;
                for (auto i : opcode_table[opcode]) {
                    if (i.match(tokens)) {
                        machine_code.push_back(i.get_opcode());
                        match_found = true;

                        auto operands = i.get_operands();

                        /*
                         * Argument parser.
                         * -------------------------------------------------- */
                        for (int j = 0; j < tokens.size(); ++j) {
                            if (operands[j].get_operand_type() == "ADDRESS") {
                                if (tokens[j].back() == 'h' ||
                                    tokens[j].back() == 'H') {

                                    tokens[j].pop_back();
                                    int address = hex_to_int(tokens[j]);
                                    if (0 <= address && address <= 0xFFFF) {
                                        machine_code.push_back(
                                            tokens[j].substr(2,2)
                                        );
                                        machine_code.push_back(
                                            tokens[j].substr(0,2)
                                        );
                                    }
                                }
                                else {
                                    int address = stoi(tokens[j]);
                                    if (0 <= address && address <= 0xFFFF) {
                                        int l_byte = address % 0xFF;
                                        int h_byte = address / 0xFF;

                                        machine_code.push_back(
                                            to_hex_string(l_byte)
                                        );
                                        machine_code.push_back(
                                            to_hex_string(h_byte)
                                        );
                                    }
                                }
                            }

                            if (operands[j].get_operand_type() == "LABEL") {
                                int address = 0;
                                if (symbol_table.count(tokens[j])) {
                                    address = symbol_table[tokens[j]];
                                }
                                else {
                                    std::cout << "Invalid reference to symbol"
                                        << tokens[j] << std::endl;

                                    error_data[line_no] =
                                        SYMBOL_NOT_FOUND_ERROR;
                                }

                                if (0 <= address && address <= 0xFFFF) {
                                    int l_byte = address % 0xFF;
                                    int h_byte = address / 0xFF;

                                    machine_code.push_back(
                                        to_hex_string(l_byte)
                                    );
                                    machine_code.push_back(
                                        to_hex_string(h_byte)
                                    );
                                }
                            }

                            if (operands[j].get_operand_type() == "DATA") {
                                if (tokens[j].back() == 'h' ||
                                    tokens[j].back() == 'H') {

                                    tokens[j].pop_back();
                                    machine_code.push_back(tokens[j]);
                                }
                                else {
                                    machine_code.push_back(
                                        to_hex_string(stoi(tokens[j]))
                                    );
                                }
                            }

                            /* End of parser */
                        }

                        break;
                    }
                }

                if (!match_found) {
                    std::cout << "Error: invalid operation on line:" << line_no
                              << std::endl;

                    error_data[line_no] = OPCODE_NOT_FOUND_ERROR;
                    break;
                }
            }
            else {
                std::cout << "Error: invalid operation \"" + opcode + "\"\n";
                error_data[line_no] = OPCODE_NOT_FOUND_ERROR;
                break;
            }
        }

        machine_code.push_back("-");

        ++line_no;
    }
}

void Assembler::assemble(std::string asm_code_file) {
    pass_one(asm_code_file);
    pass_two(asm_code_file);
}

void Assembler::print_machine_code() {
    for (auto i : machine_code) {
        std::cout << i << "\n";
    }
    std::cout << std::endl;
}

std::unordered_map<std::string, int> Assembler::get_symbol_table() {
    return symbol_table;
}

std::unordered_map<std::string, int> Assembler::get_simulator_symbol_table() {
    return simulator_symbol_table;
}

std::unordered_map<std::string, std::vector<Opcode>>
Assembler::get_opcode_table() {
    return opcode_table;
}

std::vector<std::string> Assembler::get_machine_code() {
    return machine_code;
}

std::map<int,int> Assembler::get_error_data() {
    return error_data;
}

void Assembler::clean() {
    symbol_table.clear();
    simulator_symbol_table.clear();
    machine_code.clear();
    pass_one_completed = false;
    error_data.clear();
}
