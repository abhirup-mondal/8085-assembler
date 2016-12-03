#include <iostream>

#include "../opcode.hpp"
#include "../simulator.hpp"
#include "../string_cleaner.hpp"

int Simulator::lda(std::vector<std::string> args) {
    if (args.size() != 1) {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    int address;
    if ((address=parse_address(args[0])) == -1) {
        assembler.error_data[registers["PC"]] = ADDRESS_OUT_OF_RANGE_ERROR;
		simulator_error_data[registers["PC"]] = ADDRESS_OUT_OF_RANGE_ERROR;
		return false;
    }
    accessed_memory.insert(address);
    registers["A"] = memory[address];
    ++registers["PC"];
    return true;
}

int Simulator::ldax(std::vector<std::string> args) {
    if (args.size() != 1) {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    int address;
    if(args[0] == "B")
            address = (registers["B"] << 8) + registers["C"];
    else if(args[0] == "D")
            address = (registers["D"] << 8) + registers["E"];
    else {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
        simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
        return false;
    }
    accessed_memory.insert(address);
    registers["A"] = memory[address];
    ++registers["PC"];
    return true;
}

int Simulator::lhld(std::vector<std::string> args) {
    if (args.size() != 1) {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    int address;
    if((address=parse_address(args[0])) == -1 || address==0xFFFF) {
        assembler.error_data[registers["PC"]] = ADDRESS_OUT_OF_RANGE_ERROR;
		simulator_error_data[registers["PC"]] = ADDRESS_OUT_OF_RANGE_ERROR;
		return false;
    }
    accessed_memory.insert(address);
    accessed_memory.insert(address+1);
    registers["L"] = memory[address];
    registers["H"] = memory[address+1];
    ++registers["PC"];
    return true;
}

int Simulator::lxi(std::vector<std::string> args) {
    if (args.size() != 2 || !is_register(args[0])) {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    int address=parse_address(args[1]);
    if (address == -1) {
        assembler.error_data[registers["PC"]] = DATA_OUT_OF_RANGE_ERROR;
		simulator_error_data[registers["PC"]] = DATA_OUT_OF_RANGE_ERROR;
		return false;
    }
    if (args[0] == "H") {
        registers["H"] = ((address >> 8) & 0xFF) ;
        registers["L"] = address & 0xFF;
    }
    else if (args[0] == "B") {
        registers["B"] = ((address >> 8) & 0xFF) ;
        registers["C"] = address & 0xFF;
    }
    else if (args[0] == "D") {
        registers["D"] = ((address >> 8) & 0xFF) ;
        registers["E"] = address & 0xFF;
    }
    else if (args[0] == "SP") {
        registers["SP"] = address;
    }
    else {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
        return false;
    }
    ++registers["PC"];
    return true;
}

int Simulator::mov(std::vector<std::string> args) {
    if(args.size() == 2) {
        int address;
        if(is_register(args[0]) && args[1] == "M") {
            address = (registers["H"] << 8) + registers["L"];
            accessed_memory.insert(address);
            registers[args[0]] = memory[address];
            ++registers["PC"];
            return true;
        }
        if(is_register(args[1]) && args[0] == "M") {
            address = (registers["H"] << 8) + registers["L"];
            accessed_memory.insert(address);
            memory[address] = registers[args[1]];
            ++registers["PC"];
            return true;
        }
        if(is_register(args[0]) && is_register(args[1])) {
            registers[args[0]] = registers[args[1]];
            ++registers["PC"];
            return true;
        }
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    else {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
}

int Simulator::mvi(std::vector<std::string> args) {
    if(args.size() == 2) {
        int data = parse_data(args[1]);
        if(is_register(args[0]) && (data != -1)) {
            registers[args[0]] = data;
            ++registers["PC"];
            return true;
        }
        if(args[0] == "M" && (data != -1)) {
            int address = (registers["H"] << 8) + registers["L"];
            accessed_memory.insert(address);
            memory[address] = data;
            ++registers["PC"];
            return true;
        }
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    else {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
}

int Simulator::pop(std::vector<std::string> args) {
	if(args.size() != 1) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}

	if(args[0] == "B") {
        accessed_memory.insert(registers["SP"]);
        registers["C"] = memory[registers["SP"] ++];
        accessed_memory.insert(registers["SP"]);
        registers["B"] = memory[registers["SP"] ++];
        ++registers["PC"];
		return true;
	}
	else if(args[0] == "D") {
        accessed_memory.insert(registers["SP"]);
		registers["E"] = memory[registers["SP"] ++];
        accessed_memory.insert(registers["SP"]);
		registers["D"] = memory[registers["SP"] ++];
		++registers["PC"];
		return true;
	}
	else if(args[0] == "H") {
        accessed_memory.insert(registers["SP"]);
		registers["L"] = memory[registers["SP"] ++];
        accessed_memory.insert(registers["SP"]);
		registers["H"] = memory[registers["SP"] ++];
		++registers["PC"];
		return true;
	}
	else if(args[0] == "PSW") {
        accessed_memory.insert(registers["SP"]);
		int flag_reg = memory[registers["SP"] ++];

		AC = flag_reg & 1;
		flag_reg = flag_reg >> 1;

		C = flag_reg & 1;
		flag_reg = flag_reg >> 1;

		Z = flag_reg & 1;
		flag_reg = flag_reg >> 1;

		P = flag_reg & 1;
		flag_reg = flag_reg >> 1;

		S = flag_reg & 1;

        accessed_memory.insert(registers["SP"]);
		registers["A"] = memory[registers["SP"] ++];
		++registers["PC"];
		return true;
	}

	else {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
}

int Simulator::push(std::vector<std::string> args) {
	if(args.size() != 1) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}

	if(args[0] == "B") {
		memory[--registers["SP"]] = registers["B"];
        accessed_memory.insert(registers["SP"]);
		memory[--registers["SP"]] = registers["C"];
        accessed_memory.insert(registers["SP"]);
		++registers["PC"];
		return true;
	}
	else if(args[0] == "D") {
		memory[--registers["SP"]] = registers["D"];
        accessed_memory.insert(registers["SP"]);
		memory[--registers["SP"]] = registers["E"];
        accessed_memory.insert(registers["SP"]);
		++registers["PC"];
		return true;
	}
	else if(args[0] == "H") {
		memory[--registers["SP"]] = registers["H"];
        accessed_memory.insert(registers["SP"]);
		memory[--registers["SP"]] = registers["L"];
        accessed_memory.insert(registers["SP"]);
		++registers["PC"];
		return true;
	}
	else if(args[0] == "PSW") {
		int temp_register = S;
		temp_register = (temp_register << 1) + P;
		temp_register = (temp_register << 1) + Z;
		temp_register = (temp_register << 1) + C;
		temp_register = (temp_register << 1) + AC;

		memory[--registers["SP"]] = registers["A"];
        accessed_memory.insert(registers["SP"]);
		memory[--registers["SP"]] = temp_register;
        accessed_memory.insert(registers["SP"]);
		++registers["PC"];
		return true;
	}

	else {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
}

int Simulator::shld(std::vector<std::string> args) {
    if (args.size() != 1) {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    int address;
    if((address=parse_address(args[0])) == -1 || address==0xFFFF) {
        assembler.error_data[registers["PC"]] = ADDRESS_OUT_OF_RANGE_ERROR;
		simulator_error_data[registers["PC"]] = ADDRESS_OUT_OF_RANGE_ERROR;
		return false;
    }
    accessed_memory.insert(address);
    accessed_memory.insert(address+1);
    memory[address] = registers["L"];
    memory[address+1] = registers["H"];
    ++registers["PC"];
    return true;
}

int Simulator::sta(std::vector<std::string> args) {
    if (args.size() != 1) {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    int address;
    if ((address=parse_address(args[0])) == -1) {
        assembler.error_data[registers["PC"]] = ADDRESS_OUT_OF_RANGE_ERROR;
		simulator_error_data[registers["PC"]] = ADDRESS_OUT_OF_RANGE_ERROR;
		return false;
    }
    accessed_memory.insert(address);
    memory[address] = registers["A"];
    ++registers["PC"];
    return true;
}

int Simulator::stax(std::vector<std::string> args) {
    if (args.size() != 1) {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    int address;
    if(args[0] == "B")
            address = (registers["B"] << 8) + registers["C"];
    else if(args[0] == "D")
            address = (registers["D"] << 8) + registers["E"];
    else {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
        simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
        return false;
    }
    accessed_memory.insert(address);
    memory[address] = registers["A"];
    ++registers["PC"];
    return true;
}

int Simulator::xchg(std::vector<std::string> args) {
    if(args.size()) {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
    	simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
    	return false;
    }
    int temp_h = registers["H"];
    int temp_l = registers["L"];
    registers["H"] = registers["D"];
    registers["L"] = registers["E"];
    registers["D"] = temp_h;
    registers["E"] = temp_l;
    ++registers["PC"];
    return true;
}
