#include <iostream>

#include "../opcode.hpp"
#include "../simulator.hpp"
#include "../string_cleaner.hpp"

int Simulator::aci(std::vector<std::string> args) {
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
    registers["A"] += data + C;
    C = (registers["A"] > 0xFF) ? true : false;
    registers["A"] &= 0xFF;
	set_flags("A");
    ++registers["PC"];
    return true;
}


int Simulator::adc(std::vector<std::string> args) {
	if(args.size() != 1) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    if (args[0] == "M") {
    	int address = (registers["H"] << 8) + registers["L"];
        accessed_memory.insert(address);
    	registers["A"] += memory[address] + C;
    }
	else if(!is_register(args[0])) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    else registers["A"] += registers[args[0]] + C;
    C = (registers["A"] > 0xFF) ? true : false;
    registers["A"] &= 0xFF;
    set_flags("A");
    ++registers["PC"];
    return true;
}

int Simulator::add(std::vector<std::string> args) {
    if(args.size() != 1) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    if (args[0] == "M") {
    	int address = (registers["H"] << 8) + registers["L"];
        accessed_memory.insert(address);
    	registers["A"] += memory[address];
    }
	else if(!is_register(args[0])) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    else registers["A"] += registers[args[0]];
    C = (registers["A"] > 0xFF) ? true : false;
    registers["A"] &= 0xFF;
    set_flags("A");
    ++registers["PC"];
    return true;
}

int Simulator::adi(std::vector<std::string> args) {
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
    registers["A"] += data;
    C = (registers["A"] > 0xFF) ? true : false;
    registers["A"] &= 0xFF;
	set_flags("A");
    ++registers["PC"];
    return true;
 }

 int Simulator::daa(std::vector<std::string> args) {
     if(args.size()) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	 }
     if ((registers["A"] & 0x0F) > 9 || AC) {
         registers["A"] += 6;
         AC = 1;
     }
     else AC = 0;
     if (registers["A"] > 0x9F || C) {
         registers["A"] += 0x60;
         C = 1;
     }
     else C = 0;
     set_flags("A");
      ++registers["PC"];
      return true;
 }

int Simulator::dad(std::vector<std::string> args) {
     if(args.size() == 1) {
        int data = (registers["H"] << 8) + registers["L"];
        if(args[0] == "B") {
            int data1 = (registers["B"] << 8) + registers["C"];
            data += data1;
        }
        else if(args[0] == "D") {
            int data1 = (registers["D"] << 8) + registers["E"];
            data += data1;
        }
        else if(args[0] == "H") data += data;
        C = (data & 0x10000) ? true : false;
        registers["L"] = data & 0xFF;
        registers["H"] = (data >> 8) & 0xFF;
        ++registers["PC"];
        return true;
    }
	assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
	simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
	return false;
 }

 int Simulator::dcr(std::vector<std::string> args) {
    if(args.size() != 1) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    if (args[0] == "M") {
    	int address = (registers["H"] << 8) + registers["L"];
        accessed_memory.insert(address);
    	--memory[address] &= 0xFF;
    }
    else if (!is_register(args[0])) {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    else --registers[args[0]] &= 0xFF;
    AC = ( (registers[args[0]] & 0x0F) == 0x0F) ? 1 : 0;
    set_flags(args[0]);
     ++registers["PC"];
     return true;
}

int Simulator::dcx(std::vector<std::string> args) {
    if(args.size() == 1) {
        if(args[0] == "B") {
            int data = (registers["B"] << 8) + registers["C"] - 1;
            registers["C"] = data & 0xFF;
            registers["B"] = (data >> 8) & 0xFF;
        }
        else if(args[0] == "H") {
            int data = (registers["H"] << 8) + registers["L"] - 1;
            registers["L"] = data & 0xFF;
            registers["H"] = (data >> 8) & 0xFF;
        }
        else if(args[0] == "D") {
            int data = (registers["D"] << 8) + registers["E"] - 1;
            registers["E"] = data & 0xFF;
            registers["D"] = (data >> 8) & 0xFF;
        }
        ++registers["PC"];
        return true;
    }
    assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
	simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
	return false;
}

int Simulator::inr(std::vector<std::string> args) {
    if(args.size() != 1) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    if (args[0] == "M") {
    	int address = (registers["H"] << 8) + registers["L"];
        accessed_memory.insert(address);
    	++memory[address] &= 0xFF;
    }
    else if (!is_register(args[0])) {
        assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
    }
    else ++registers[args[0]] &= 0xFF;
    AC = ( (registers[args[0]] & 0x0F) == 0x00) ? 1 : 0;
    set_flags(args[0]);
    ++registers["PC"];
    return true;
}

int Simulator::inx(std::vector<std::string> args) {
    if(args.size() == 1) {
        if(args[0] == "B") {
            if(++registers["C"] > 0xFF) {
                registers["C"] &= 0xFF;
                registers["B"] = (registers["B"] + 1) & 0xFF;
            }
        }
        else if(args[0] == "H") {
            if(++registers["L"] > 0xFF) {
                registers["L"] &= 0xFF;
                registers["H"] = (registers["H"] + 1) & 0xFF;
            }
        }
        else if(args[0] == "D") {
            if(++registers["E"] > 0xFF) {
                registers["E"] &= 0xFF;
                registers["D"] = (registers["D"] + 1) & 0xFF;
            }
        }
        ++registers["PC"];
        return true;
    }
	assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
	simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
	return false;
}

int Simulator::sbb(std::vector<std::string> args) {
	if(args.size() != 1) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    if(args[0] == "M") {
    	int address = (registers["H"]<<8) + registers["L"];
        accessed_memory.insert(address);
    	registers["A"] -= memory[address] + C;
    }
	else if(!is_register(args[0])) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    else registers["A"] -= registers[args[0]] + C;
    C = (registers["A"] < 0x00) ? true : false;
	registers["A"] &= 0xFF;
	set_flags("A");
    ++registers["PC"];
    return true;
}

int Simulator::sbi(std::vector<std::string> args) {
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
    registers["A"] -= data + C;
    C = (registers["A"] < 0x00) ? true : false;
    registers["A"] &= 0xFF;
	set_flags("A");
    ++registers["PC"];
    return true;
}

int Simulator::sub(std::vector<std::string> args) {
    if(args.size() != 1) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    if(args[0] == "M") {
    	int address = (registers["H"]<<8) + registers["L"];
        accessed_memory.insert(address);
    	registers["A"] -= memory[address];
    }
	else if(!is_register(args[0])) {
		assembler.error_data[registers["PC"]] = SYNTAX_ERROR;
		simulator_error_data[registers["PC"]] = SYNTAX_ERROR;
		return false;
	}
    else registers["A"] -= registers[args[0]];
    C = (registers["A"] < 0x00) ? true : false;
	registers["A"] &= 0xFF;
	set_flags("A");
    ++registers["PC"];
    return true;
}

int Simulator::sui(std::vector<std::string> args) {
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
    registers["A"] -= data;
    C = (registers["A"] < 0x00) ? true : false;
    registers["A"] &= 0xFF;
	set_flags("A");
    ++registers["PC"];
    return true;
}
