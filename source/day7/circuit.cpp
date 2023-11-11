#include "../global_in.hpp"

static const std::string RES_DAY7 = "resources/day7/input.txt";

static const size_t MEM_SIZE = 1000;

namespace Circuit {
    enum Operation{
        IMM,
        NOT,
        AND,
        OR,
        RSHIFT,
        LSHIFT,
    };

    struct Pin{
        bool calculated = false;
        size_t mem_idx;

        Pin *arg1;
        Pin *arg2;
        Operation op = IMM;

    };
    
    static void perform_calc(Pin &pin, uint16_t *mem){
        if(pin.calculated) return;

        if( !(pin.arg1->calculated) ){
            perform_calc(*pin.arg1, mem);
        }

        if( (pin.op != IMM) && (pin.op != NOT) && !(pin.arg2->calculated)){
            perform_calc(*pin.arg2, mem);
        }

        switch (pin.op) {
            case IMM:
                mem[pin.mem_idx] = mem[pin.arg1->mem_idx];
                break;
            case NOT:
                mem[pin.mem_idx] = ~mem[pin.arg1->mem_idx];
                break;
            case AND:
                mem[pin.mem_idx] = mem[pin.arg1->mem_idx] & mem[pin.arg2->mem_idx];
                break;
            case OR:
                mem[pin.mem_idx] = mem[pin.arg1->mem_idx] | mem[pin.arg2->mem_idx];
                break;
            case RSHIFT:
                mem[pin.mem_idx] = mem[pin.arg1->mem_idx] >> mem[pin.arg2->mem_idx];
                break;
            case LSHIFT:
                mem[pin.mem_idx] = mem[pin.arg1->mem_idx] << mem[pin.arg2->mem_idx];
                break;
        }

        pin.calculated = true;
    }

    static std::string read_str(std::ifstream &file, const char delim){
        std::string str;
        
        char c = file.get();
        while(c != delim && !file.eof()){
            str.push_back(c);
            //if(file.eof()) return str;
            c = file.get();
        }

        return str;
    }

    static bool str_equals(const std::string &str, const std::string &str2){

        if(str.length() != str2.length()) return false;

        for(size_t i = 0; i<str.length(); i++){
            if (str[i] != str2[i]) return false;
        }

        return true;
    }

    static bool is_command(const std::string &str){

        if(str_equals(str, "AND") || str_equals(str, "OR") || str_equals(str, "NOT") || str_equals(str, "RSHIFT") || str_equals(str, "LSHIFT") ){
            return true;
        }

        return false;
    }

    static bool is_num(const std::string &str){
        for(size_t idx = 0; idx<str.length(); idx++){
            if(str[idx]<'0' || str[idx]>'9') return false;
        }

        return true;
    }

    static uint16_t get_str_num(const std::string &str){
        uint16_t num = 0;
        for(size_t idx = 0; idx<str.length(); idx++){
            num *= 10;
            num += (str[idx]-'0');
        }

        return num;
    }

    static Operation get_op(const std::string &str){
        switch (str[0]) {
            case 'A':
                return AND;
            case 'O':
                return OR;
            case 'N':
                return NOT;
            case 'R':
                return RSHIFT;
            case 'L':
                return LSHIFT;
            default:
                return AND; //Should not be reached
        }
    }

    static int load_prog(std::map<std::string, Pin> &wires, uint16_t *stack){
        std::ifstream file(RES_DAY7);

        if(!file.is_open()){
            std::cout << "Failed to open file." << std::endl;
            return 1;
        }

        std::string str;
        std::string buff[3];

        size_t stack_idx = 0;

        uint32_t line_n = 0;

        while (!file.eof()) {
            
            if(stack_idx + 3 >= MEM_SIZE){
                std::cout << "Using too much memory." << std::endl;
                return 3;
            }
            line_n++;
            uint8_t arg_count = 0;

            str = read_str(file, ' ');
            if(file.eof()){
                std::cout << "Invalid line: "<< line_n << std::endl;
                return 3;
            }

            while(str != "->" && arg_count<3){
                buff[arg_count] = str;
                arg_count++;
                str = read_str(file, ' ');
                if(file.eof()){
                    std::cout << "Invalid line: "<< line_n << std::endl;
                    return 3;
                }
            }

            if(str != "->"){
                std::cout << "Too many arguments before assigning to pin; line: " << line_n << std::endl;
                return 2;
            }

            if(arg_count == 0){
                std::cout << "Missing arguments; line: " << line_n << std::endl;
                return 2;
            }

            str = read_str(file, '\n');

            if(arg_count == 1){
                if(is_command(buff[0])){
                    std::cout << "Illegal Operation; line: "<< line_n << std::endl;
                    return 2;
                }

                if(is_num(buff[0])){
                    stack[stack_idx] = get_str_num(buff[0]);

                    wires[str] = Pin{
                        .calculated = true,
                        .mem_idx = stack_idx,
                    };

                    stack_idx++;
                    continue;
                }

                if(wires.count(buff[0]) == 0){
                    wires[buff[0]] = Pin{
                        .calculated = false,
                    };
                }

                wires[str] = Pin{
                    .calculated = false,
                    .mem_idx = stack_idx,
                    .arg1 = &wires[buff[0]],
                    .op = IMM,
                };

                stack_idx++;
                continue;
            }

            if(arg_count == 2){
                if(!str_equals(buff[0], "NOT") || is_command(buff[1])){
                    std::cout << "Illegal Operation; line: "<< line_n << std::endl;
                    return 2;
                }

                if(is_num(buff[1])){
                    uint16_t val = get_str_num(buff[1]);
                    stack[stack_idx] = !val;

                    wires[str] = Pin{
                        .calculated = true,
                        .mem_idx = stack_idx,
                    };

                    stack_idx++;
                    continue;
                }

                if(wires.count(buff[1]) == 0){
                    wires[buff[1]] = Pin{
                        .calculated = false,
                    };
                }

                wires[str] = Pin{
                    .calculated = false,
                    .mem_idx = stack_idx,
                    .arg1 = &wires[buff[1]],
                    .op = NOT,
                };

                stack_idx++;
                continue;
            }

            if(arg_count == 3){
                if( !is_command(buff[1]) || is_command(buff[0]) || is_command(buff[2])){
                    std::cout << "Illegal Operation; line: "<< line_n << std::endl;
                    return 2;
                }

                if(wires.count(buff[0]) == 0) {
                    if(is_num(buff[0])){
                        stack[stack_idx] = get_str_num(buff[0]);

                        wires[buff[0]] = Pin{
                            .calculated = true,
                            .mem_idx = stack_idx,
                        };
                        stack_idx++;
                    }else{
                        wires[buff[0]] = Pin{
                            .calculated = false,
                        };
                    }

                }

                if(wires.count(buff[2]) == 0) {
                    if(is_num(buff[2])){
                        stack[stack_idx] = get_str_num(buff[2]);

                        wires[buff[2]] = Pin{
                            .calculated = true,
                            .mem_idx = stack_idx,
                        };
                        stack_idx++;
                    }else{
                        wires[buff[2]] = Pin{
                            .calculated = false,
                        };
                    }

                }

                wires[str] = Pin{
                    .calculated = false,
                    .mem_idx = stack_idx,
                    .arg1 = &wires[buff[0]],
                    .arg2 = &wires[buff[2]],
                    .op = get_op(buff[1]),
                };

                stack_idx++;
                continue;
            }
        }

        return 0;
    }

    static void reset_wires(std::map<std::string, Pin> &wires){
        std::map<std::string, Pin>::iterator it;
        for(it = wires.begin(); it != wires.end(); it++){
            if(it->second.arg1 != 0){
                it->second.calculated = false;
            }
        }
    }

    static void simulate(std::map<std::string, Pin> &wires, uint16_t *mem){
        std::map<std::string, Pin>::iterator it;
        for(it = wires.begin(); it != wires.end(); it++){
            perform_calc(it->second, mem);
        }
    }

    void write_answers(){
        uint16_t mem[MEM_SIZE];
        std::map<std::string, Pin> wires;

        load_prog(wires, mem);
        Pin *pinA = &wires.find("a")->second;
        

        simulate(wires, mem);
        std::cout << "\tFirst 'a' : "<< mem[ pinA->mem_idx ] << std::endl;

        reset_wires(wires);

        Pin *pinB = &wires.find("b")->second;
        mem[pinB->mem_idx] = mem[pinA->mem_idx];
        pinB->op = IMM;

        simulate(wires, mem);
        std::cout << "\tSecond 'a' : "<< mem[ wires.find("a")->second.mem_idx ] << std::endl;
        
        /*std::map<std::string, Pin>::iterator it;
        for(it = wires.begin(); it != wires.end(); it++){
            std::cout << it->first << ": " << mem[it->second.mem_idx] << std::endl;
        }*/
    }
}
