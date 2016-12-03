#include <iostream>

#include "../opcode.hpp"
#include "../simulator.hpp"
#include "../string_cleaner.hpp"

int Simulator::call(std::vector<std::string> args) {
	if(args.size() != 1) {
    	assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
	int subroutine_address = parse_label(args[0]);
	push(std::vector<std::string>(1, "PSW"));
    --registers["PC"];
	push(std::vector<std::string>(1, "B"));
    --registers["PC"];
    push(std::vector<std::string>(1, "D"));
    --registers["PC"];
    push(std::vector<std::string>(1, "H"));
    memory[--registers["SP"]] = --registers["PC"];
    accessed_memory.insert(registers["SP"]);
	registers["PC"] = subroutine_address;
    return true;
}

int Simulator::cc(std::vector<std::string> args) {
	if(C) return call(args);
	++registers["PC"];
	return true;
}

int Simulator::cm(std::vector<std::string> args) {
	if(S) return call(args);
	++registers["PC"];
	return true;
}

int Simulator::cnc(std::vector<std::string> args) {
	if(!C) return call(args);
	++registers["PC"];
	return true;
}

int Simulator::cnz(std::vector<std::string> args) {
	if(!Z) return call(args);
	++registers["PC"];
	return true;
}

int Simulator::cp(std::vector<std::string> args) {
	if(!S) return call(args);
	++registers["PC"];
	return true;
}

int Simulator::cpe(std::vector<std::string> args) {
	if(P) return call(args);
	++registers["PC"];
	return true;
}

int Simulator::cpo(std::vector<std::string> args) {
	if(!P) return call(args);
	++registers["PC"];
	return true;
}

int Simulator::cz(std::vector<std::string> args) {
	if(Z) return call(args);
	++registers["PC"];
	return true;
}

int Simulator::hlt(std::vector<std::string> args) {
    if(args.size()) {
    	assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    simulation_completed = true;
    return true;
}

int Simulator::jc(std::vector<std::string> args) {
    if (C) return jmp(args);
    ++registers["PC"];
    return true;
}

int Simulator::jm(std::vector<std::string> args) {
    if (S) return jmp(args);
    ++registers["PC"];
    return true;
}


int Simulator::jmp(std::vector<std::string> args) {
    int label;
    if(args.size() != 1 || (label = parse_label(args[0]))==-1) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    registers["PC"] = label;
    return true;
}

int Simulator::jnc(std::vector<std::string> args) {
    if (!C) return jmp(args);
    ++registers["PC"];
    return true;
}

int Simulator::jnz(std::vector<std::string> args) {
    if (!Z) return jmp(args);
    ++registers["PC"];
    return true;
}

int Simulator::jp(std::vector<std::string> args) {
    if (!S) return jmp(args);
    ++registers["PC"];
    return true;
}

int Simulator::jpe(std::vector<std::string> args) {
    if (P) return jmp(args);
    ++registers["PC"];
    return true;
}

int Simulator::jpo(std::vector<std::string> args) {
    if (!P) return jmp(args);
    ++registers["PC"];
    return true;
}

int Simulator::jz(std::vector<std::string> args) {
    if (Z) return jmp(args);
    ++registers["PC"];
    return true;
}

int Simulator::nop(std::vector<std::string> args) {
    if(args.size()) {
    	assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    ++registers["PC"];
    return true;
}

int Simulator::pchl(std::vector<std::string> args) {
    if(args.size()) {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    int next_address = (registers["H"] << 8) + registers["L"];
    registers["PC"] = next_address;
    return true;
}

int Simulator::rc(std::vector<std::string> args) {
	if(C) return ret(args);
	++registers["PC"];
	return true;
}

int Simulator::ret(std::vector<std::string> args) {
    if(args.size() != 0) {
    	assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
	accessed_memory.insert(registers["SP"]);
    registers["PC"] = memory[registers["SP"]++];
    pop(std::vector<std::string>(1, "H"));
    --registers["PC"];
	pop(std::vector<std::string>(1, "D"));
    --registers["PC"];
    pop(std::vector<std::string>(1, "B"));
    --registers["PC"];
    pop(std::vector<std::string>(1, "PSW"));
    return true;
}

int Simulator::rm(std::vector<std::string> args) {
	if(S) return ret(args);
	++registers["PC"];
	return true;
}

int Simulator::rnc(std::vector<std::string> args) {
    if(!C) return ret(args);
	++registers["PC"];
	return true;
}

int Simulator::rnz(std::vector<std::string> args) {
	if(!Z) return ret(args);
	++registers["PC"];
	return true;
}

int Simulator::rp(std::vector<std::string> args) {
	if(!S) return ret(args);
	++registers["PC"];
	return true;
}

int Simulator::rpe(std::vector<std::string> args) {
	if(P) return ret(args);
	++registers["PC"];
	return true;
}

int Simulator::rpo(std::vector<std::string> args) {
	if(!P) return ret(args);
	++registers["PC"];
	return true;
}

int Simulator::rst(std::vector<std::string> args) {
    /* Not looking into the argument and treating everything to be the same */
    if(args.size() == 1 && parse_data(args[0]) != -1) {
        simulation_completed = true;
        return true;
    }
    else {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
}

int Simulator::rz(std::vector<std::string> args) {
	if(Z) return ret(args);
	++registers["PC"];
	return true;
}

int Simulator::sphl(std::vector<std::string> args) {
    if(args.size()) {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    int next_address = (registers["H"] << 8) + registers["L"];
    registers["SP"] = next_address;
    return true;
}

int Simulator::xthl(std::vector<std::string> args) {
	if(args.size()) {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    int temp_h = registers["H"];
    int temp_l = registers["L"];
    int address = registers["SP"];
    accessed_memory.insert(address);
    registers["L"] = memory[address];
    accessed_memory.insert(address+1);
    registers["H"] = memory[address + 1];
    memory[address] = temp_l;
    memory[address + 1] = temp_h;
    ++registers["PC"];
    return true;
}
