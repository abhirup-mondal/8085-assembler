#ifndef _GUARD_OPCODE_HPP
#define _GUARD_OPCODE_HPP

#include <set>
#include <string>
#include <unordered_map>
#include <vector>


static const
std::set<std::string> REGISTERS =
    {
        "A", "B", "C", "D", "E",
        "H", "L", "SP", "PC",
    };

class Operand {
private:
    std::string operand_type;
    std::string operand;

public:
    Operand(std::string ot, std::string op);
    bool match(std::string operand);
    void print();

    std::string get_operand_type();
};


class Opcode {
private:
    std::string          opcode;
    short                byte_count;
    std::string          mnemonic;
    std::vector<Operand> operands;

public:
    Opcode(std::string mn = "", std::string oc = "", short bc = 0);
    void add_operand(std::string op_data);
    bool match(std::vector<std::string> operands);

    std::string get_opcode();
    short get_byte_count();
    std::vector<Operand> get_operands();

    void print();
};


std::unordered_map<std::string, std::vector<Opcode>>
build_opcode_table(std::string filename = "data/opcode_table.txt");


#endif
