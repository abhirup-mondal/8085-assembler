#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

#include "opcode.hpp"
#include "string_cleaner.hpp"

Operand::Operand(std::string ot, std::string op) {
    operand_type = ot;
    operand      = op;
}

void Operand::print() {
    std::cout << operand_type << ": " << operand << std::endl;
}

bool Operand::match(std::string operand) {
    if (operand_type == "REGISTER" || operand_type == "CODE") {
        if (this->operand == operand) {
            return true;
        }
        else {
            return false;
        }
    }

    return true;
}

std::string Operand::get_operand_type() {
    return operand_type;
}

Opcode::Opcode(std::string mn, std::string oc, short bc) {
    mnemonic     = mn;
    opcode       = oc;
    byte_count   = bc;
}

void Opcode::add_operand(std::string op_data) {
    if (REGISTERS.count(op_data)) {
        Operand temp = Operand("REGISTER", op_data);
        operands.push_back(temp);
    }
    else if (opcode == "RST") {
        Operand temp = Operand("CODE", op_data);
        operands.push_back(temp);
    }
    else {
        Operand temp = Operand(op_data, "");
        operands.push_back(temp);
    }
}

void Opcode::print() {
    std::cout << mnemonic << " " << opcode << std::endl;
    for (auto operand : operands) {
        operand.print();
    }
}

bool Opcode::match(std::vector<std::string> op) {
    if (operands.size() == op.size()) {
        for (int i = 0; i < operands.size(); ++i) {
            if (!this->operands[i].match(op[i])) {
                return false;
            }
        }

        return true;
    }
    else {
        return false;
    }
}

std::string Opcode::get_opcode() {
    return opcode;
}

short Opcode::get_byte_count() {
    return byte_count;
}

std::vector<Operand> Opcode::get_operands() {
    return operands;
}


std::unordered_map<std::string, std::vector<Opcode>>
build_opcode_table(std::string filename) {
    std::unordered_map<std::string, std::vector<Opcode>> table;
    std::ifstream data_file(filename);

    std::string data_line;
    while (std::getline(data_file, data_line)) {
        whitespace_clean(data_line);
        std::vector<std::string> tokens = split(data_line, ' ');

        Opcode current;
        if (tokens.size()) {
            current = Opcode(tokens[2], tokens[0], stoi(tokens[1]));
        }

        for (int i = 3; i < tokens.size(); ++i) {
            current.add_operand(tokens[i]);
        }

        table[tokens[2]].push_back(current);
    }

    return table;
}
