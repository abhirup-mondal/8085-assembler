#include <iostream>

#include "../opcode.hpp"
#include "../simulator.hpp"
#include "../string_cleaner.hpp"

 int Simulator::ana(std::vector<std::string> args) {
    if(args.size() != 1) {
 		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
 		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
 		return false;
 	}
    if (args[0] == "M") {
        int address = (registers["H"] << 8) + registers["L"];
        accessed_memory.insert(address);
        registers["A"] &= memory[address];
    }
 	else if(!is_register(args[0])) {
 		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
 		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
 		return false;
 	}
    else registers["A"] &= registers[args[0]];
    C = 0;
    AC = 1;
    set_flags("A");
    ++registers["PC"];
    return true;
}

int Simulator::ani(std::vector<std::string> args) {
    if(args.size() != 1) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    int data;
    if ((data=parse_data(args[0])) == -1) {
        assembler.error_data[registers["PC"]] = DATA_OUT_OF_RANGE_ERROR;
		simulator_error_data[registers["PC"]] = DATA_OUT_OF_RANGE_ERROR;
		return false;
    }
    registers["A"] &= data;
    C = 0;
    AC = 1;
	set_flags("A");
    ++registers["PC"];
    return true;
 }

 int Simulator::cma(std::vector<std::string> args) {
     if(args.size()) {
    	assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    registers["A"] = 0xFF - registers["A"];
    ++registers["PC"];
    return true;
}

 int Simulator::cmc(std::vector<std::string> args) {
    if(args.size()) {
    	assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    C = !C;
    ++registers["PC"];
    return true;
 }

 int Simulator::cmp(std::vector<std::string> args) {
    if(args.size() != 1) {
    	assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    if(args[0] == "M") {
        int address = (registers["H"] << 8) + registers["L"];
        accessed_memory.insert(address);
        if (registers["A"] < memory[address]) {
            C = 1;
            Z = 0;
        }
        else if (registers["A"] == memory[address]) {
            Z = 1;
            C = 0;
        }
        else C = Z = 0;
        ++registers["PC"];
        return true;
    }
    if(!is_register(args[0])) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
	if (registers["A"] < registers[args[0]]) {
        C = 1;
        Z = 0;
    }
    else if (registers["A"] == registers[args[0]]) {
        Z = 1;
        C = 0;
    }
    else C = Z = 0;
    ++registers["PC"];
    return true;
}

 int Simulator::cpi(std::vector<std::string> args) {
	if(args.size() != 1) {
    	assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    int data;
    if ((data=parse_data(args[0])) == -1) {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    if (registers["A"] < data) {
        C = 1;
        Z = 0;
    }
    else if (registers["A"] == data) {
        Z = 1;
        C = 0;
    }
    else {
        C = Z = 0;
    }
    ++registers["PC"];
    return true;
}

int Simulator::ora(std::vector<std::string> args) {
    if(args.size() != 1) {
 		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
 		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
 		return false;
 	}
    if (args[0] == "M") {
        int address = (registers["H"] << 8) + registers["L"];
        accessed_memory.insert(address);
        registers["A"] |= memory[address];
    }
 	else if(!is_register(args[0])) {
 		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
 		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
 		return false;
 	}
    else registers["A"] |= registers[args[0]];
    C = AC = 0;
    set_flags("A");
    ++registers["PC"];
    return true;
}

int Simulator::ori(std::vector<std::string> args) {
    if(args.size() != 1) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    int data;
    if ((data=parse_data(args[0])) == -1) {
        assembler.error_data[registers["PC"]] = DATA_OUT_OF_RANGE_ERROR;
		simulator_error_data[registers["PC"]] = DATA_OUT_OF_RANGE_ERROR;
		return false;
    }
    registers["A"] |= data;
    C = AC = 0;
	set_flags("A");
    ++registers["PC"];
    return true;
}

int Simulator::ral(std::vector<std::string> args) {
    if(args.size()) {
    	assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    bool temp_carry = C;
    C = (registers["A"] & 0x80) >> 7;
    registers["A"] = (registers["A"] << 1) & 0xFF;
    registers["A"] |= temp_carry;
    ++registers["PC"];
    return true;
}

int Simulator::rar(std::vector<std::string> args) {
    if(args.size()) {
    	assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    bool temp_carry = C;
    C = registers["A"] & 0x1;
    registers["A"] = (registers["A"] >> 1);
    registers["A"] |= (temp_carry << 7);
    ++registers["PC"];
    return true;
}

int Simulator::rlc(std::vector<std::string> args) {
    if(args.size()) {
    	assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    C = (registers["A"] & 0x80) >> 7;
    registers["A"] = (registers["A"] << 1) & 0xFF;
    registers["A"] |= C;
    ++registers["PC"];
    return true;
}

int Simulator::rrc(std::vector<std::string> args) {
    if(args.size()) {
    	assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    C = registers["A"] & 0x1;
    registers["A"] = (registers["A"] >> 1);
    registers["A"] |= (C << 7);
    ++registers["PC"];
    return true;
}

int Simulator::stc(std::vector<std::string> args) {
	if(args.size()) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
	C = 1;
	++registers["PC"];
	return true;
}

int Simulator::xra(std::vector<std::string> args) {
    if(args.size() != 1) {
 		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
 		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
 		return false;
 	}
    if (args[0] == "M") {
        int address = (registers["H"] << 8) + registers["L"];
        accessed_memory.insert(address);
        registers["A"] ^= memory[address];
    }
 	else if(!is_register(args[0])) {
 		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
 		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
 		return false;
 	}
    else registers["A"] ^= registers[args[0]];
    C = AC = 0;
    set_flags("A");
    ++registers["PC"];
    return true;
}

int Simulator::xri(std::vector<std::string> args) {
    if(args.size() != 1) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    int data;
    if ((data=parse_data(args[0])) == -1) {
        assembler.error_data[registers["PC"]] = DATA_OUT_OF_RANGE_ERROR;
		simulator_error_data[registers["PC"]] = DATA_OUT_OF_RANGE_ERROR;
		return false;
    }
    registers["A"] ^= data;
    C = AC = 0;
	set_flags("A");
    ++registers["PC"];
    return true;
}
