#ifndef _GUARD_SIMULATOR_HPP
#define _GUARD_SIMULATOR_HPP


#include <map>
#include <set>
#include <vector>

#include "assembler.hpp"

class Simulator {

    typedef int (Simulator::*asm_func_ptr)(std::vector<std::string>);

public:
    const int MEMORY = 0x10000;

    std::vector<int>             memory;
    std::map<std::string, int> registers;
    std::set<int>                accessed_memory;

    std::vector<std::vector<std::string>>          instruction_table;
    std::map<std::string, Simulator::asm_func_ptr> function_table;
    std::unordered_map<std::string, int>           symbol_table;


    Assembler assembler;

    bool assembling_completed;
    bool simulation_completed;

    /* Flags  */
    bool AC;
    bool C;
    bool P;
    bool S;
    bool Z;

    std::map<int,int> simulator_error_data;

    std::string get_error_string(int err);

    Simulator(std::string opcode_data_file = "data/opcode_table.txt");

    std::vector<int> get_memory();
    std::map<std::string, int> get_registers();

    Assembler get_assembler();

    void build_instruction_table(std::string asm_code_file);
    void build_function_table();
    void load_data_in_memory(std::string memory_data_file);

    void assemble(std::string asm_code_file);
    void simulate(std::string asm_code_file);
    int execute_instruction();

    int set_memory(int address, int value);

    void set_flags(std::string r);

    void show_output();

    void prepare_simulator(std::string asm_code_file);
    void reset_simulator();
    void reset_memory();
    void stop_simulation();
    void show_errors();

    /*
     * Each function takes a vector of string as arguments.
     * The arguments need to be parsed and then used to perform the operations.
     * There are parser functions to help the parsing.
     */

    int aci(std::vector<std::string> args);
    int adc(std::vector<std::string> args);
    int add(std::vector<std::string> args);
    int adi(std::vector<std::string> args);
    int ana(std::vector<std::string> args);
    int ani(std::vector<std::string> args);
    int call(std::vector<std::string> args); /* To be implemented later */
    int cc(std::vector<std::string> args);
    int cm(std::vector<std::string> args);
    int cma(std::vector<std::string> args);
    int cmc(std::vector<std::string> args);
    int cmp(std::vector<std::string> args);
    int cnc(std::vector<std::string> args);
    int cnz(std::vector<std::string> args);
    int cp(std::vector<std::string> args);
    int cpe(std::vector<std::string> args);
    int cpi(std::vector<std::string> args);
    int cpo(std::vector<std::string> args);
    int cz(std::vector<std::string> args);
    int daa(std::vector<std::string> args);
    int dad(std::vector<std::string> args);
    int dcr(std::vector<std::string> args);
    int dcx(std::vector<std::string> args);
    int di(std::vector<std::string> args);
    int ei(std::vector<std::string> args);
    int hlt(std::vector<std::string> args);
    int in(std::vector<std::string> args);  /* To be implemented later */
    int inr(std::vector<std::string> args);
    int inx(std::vector<std::string> args);
    int jc(std::vector<std::string> args);
    int jm(std::vector<std::string> args);
    int jmp(std::vector<std::string> args);
    int jnc(std::vector<std::string> args);
    int jnz(std::vector<std::string> args);
    int jp(std::vector<std::string> args);
    int jpe(std::vector<std::string> args);
    int jpo(std::vector<std::string> args);
    int jz(std::vector<std::string> args);
    int lda(std::vector<std::string> args);
    int ldax(std::vector<std::string> args);
    int lhld(std::vector<std::string> args);
    int lxi(std::vector<std::string> args);
    int mov(std::vector<std::string> args);
    int mvi(std::vector<std::string> args);
    int nop(std::vector<std::string> args);
    int ora(std::vector<std::string> args);
    int ori(std::vector<std::string> args);
    int out(std::vector<std::string> args);
    int pchl(std::vector<std::string> args);
    int pop(std::vector<std::string> args);
    int push(std::vector<std::string> args);
    int ral(std::vector<std::string> args);
    int rar(std::vector<std::string> args);
    int rc(std::vector<std::string> args);
    int ret(std::vector<std::string> args);
    int rim(std::vector<std::string> args);
    int rlc(std::vector<std::string> args);
    int rm(std::vector<std::string> args);
    int rnc(std::vector<std::string> args);
    int rnz(std::vector<std::string> args);
    int rp(std::vector<std::string> args);
    int rpe(std::vector<std::string> args);
    int rpo(std::vector<std::string> args);
    int rrc(std::vector<std::string> args);
    int rst(std::vector<std::string> args);
    int rz(std::vector<std::string> args);
    int sbb(std::vector<std::string> args);
    int sbi(std::vector<std::string> args);
    int shld(std::vector<std::string> args);
    int sim(std::vector<std::string> args);
    int sphl(std::vector<std::string> args);
    int sta(std::vector<std::string> args);
    int stax(std::vector<std::string> args);
    int stc(std::vector<std::string> args);
    int sub(std::vector<std::string> args);
    int sui(std::vector<std::string> args);
    int xchg(std::vector<std::string> args);
    int xra(std::vector<std::string> args);
    int xri(std::vector<std::string> args);
    int xthl(std::vector<std::string> args);

    int is_register(std::string args);
    int parse_label(std::string label);
    int parse_address(std::string address);
    int parse_data(std::string data);
};


#endif
