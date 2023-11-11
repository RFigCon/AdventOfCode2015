#include "../global_in.hpp"

#define GRID_TYPE long

namespace Lights {

    static const char ACTION_ON  = 0;
    static const char ACTION_OFF = 1;
    static const char ACTION_TOGGLE = 2;

    static const std::string RES_DAY6 = "resources/day6/";

    struct Instruction{
        char action;
        size_t x1 = 0;
        size_t y1 = 0;

        size_t x2 = 0;
        size_t y2 = 0;
    };

    struct Answers{
        int answer1 = 0;
        int answer2 = 0;
    };

    static size_t get_input(int &status, Instruction *set, const size_t SIZE){
        std::ifstream file(RES_DAY6 + "input.txt");
        status = 0;

        if(!file.is_open()){
            status = 1;
            return 0;
        }

        size_t set_idx = 0;

        char prev = 0;
        char c = 0;
        bool setting_start = true;
        bool found_comma = false;

        while(!file.eof() && set_idx<SIZE){
            prev = c;
            c = file.get();

            if(c=='\n'){
                set_idx++;

                setting_start = true;
                found_comma = false;

                continue;
            }

            if(prev == 'o'){
                switch(c){
                    case 'f':
                        set[set_idx].action = ACTION_OFF;
                        break;
                    case 'n':
                        set[set_idx].action = ACTION_ON;
                        break;
                    case 'g':
                        set[set_idx].action = ACTION_TOGGLE;
                        break;
                    case 'u':
                        setting_start = false;
                        found_comma = false;
                    default:
                        break;
                }
                continue;
            }

            if(c==','){
                found_comma = true;
                continue;
            }

            
            if(c>='0' && c<='9'){
                c -= '0';
                if(setting_start){
                    if(found_comma){
                        set[set_idx].y1 *= 10;
                        set[set_idx].y1 += c;
                    }else{
                        set[set_idx].x1 *= 10;
                        set[set_idx].x1 += c;
                    }
                }else{
                    if(found_comma){
                        set[set_idx].y2 *= 10;
                        set[set_idx].y2 += c;
                    }else{
                        set[set_idx].x2 *= 10;
                        set[set_idx].x2 += c;
                    }
                }
            }
        }

        if(set_idx>= SIZE){
            status = 2;
        }
        return set_idx;
    }

    static void execute1(GRID_TYPE *grid, const size_t GRID_LIN_SIZE, const Instruction &ins){
        for(size_t i=ins.x1; i<=ins.x2 && i<GRID_LIN_SIZE; i++){
            for(size_t j=ins.y1; j<=ins.y2 &&  j<GRID_LIN_SIZE; j++){
                size_t idx = i + j*GRID_LIN_SIZE;
                switch(ins.action){
                    case ACTION_ON:
                        grid[idx] = !((GRID_TYPE)0);
                        break;
                    case ACTION_OFF:
                        grid[idx] = 0;
                        break;
                    case ACTION_TOGGLE:
                        grid[idx] = !grid[idx];
                        break;
                    default:
                        break;
                }
            }
        }
    }

    static void part1(GRID_TYPE *grid, const size_t GRID_LINE_SIZE, const Instruction *ins_set, const size_t SET_SIZE){
        for(size_t i = 0; i<SET_SIZE; i++){
            execute1(grid, GRID_LINE_SIZE, ins_set[i]);
        }
    }

    static void execute2(GRID_TYPE *grid, const size_t GRID_LIN_SIZE, const Instruction &ins){
        for(size_t i=ins.x1; i<=ins.x2 && i<GRID_LIN_SIZE; i++){
            for(size_t j=ins.y1; j<=ins.y2 &&  j<GRID_LIN_SIZE; j++){
                size_t idx = i + j*GRID_LIN_SIZE;
                switch(ins.action){
                    case ACTION_ON:
                        grid[idx] += 1;
                        break;
                    case ACTION_OFF:
                        grid[idx] = (grid[idx]>0) ? grid[idx]-1 : 0;
                        break;
                    case ACTION_TOGGLE:
                        grid[idx] += 2;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    static void part2(GRID_TYPE *grid, const size_t GRID_LINE_SIZE, const Instruction *ins_set, const size_t SET_SIZE){
        for(size_t i = 0; i<SET_SIZE; i++){
            execute2(grid, GRID_LINE_SIZE, ins_set[i]);
        }
    }

    static int count_on(const GRID_TYPE *grid, const size_t LIN_SIZE){
        int count = 0;

        for(size_t i = 0; i<LIN_SIZE; i++){
            for(size_t j = 0; j<LIN_SIZE; j++){
                if(grid[i*LIN_SIZE + j] != 0){
                    count++;
                }
            }    
        }

        return count;
    }

    static int get_total(const GRID_TYPE *grid, const size_t LIN_SIZE){
        int count = 0;

        for(size_t i = 0; i<LIN_SIZE; i++){
            for(size_t j = 0; j<LIN_SIZE; j++){
                count += grid[i*LIN_SIZE + j];
            }    
        }

        return count;
    }

    static Answers run(){
        const size_t GRID_LIN_SIZE = 1000;
        const size_t INSTR_SET_SIZE = 500;

        GRID_TYPE *grid = new GRID_TYPE[GRID_LIN_SIZE*GRID_LIN_SIZE];
        Instruction ins_set[INSTR_SET_SIZE];

        int status = 0;
        size_t set_num = get_input(status, ins_set, INSTR_SET_SIZE);
        if(status == 1){
            std::cout << "Failed to open file." << std::endl;
            return Answers{0, 0};
        }

        if(status == 2){
            std::cout << "Input had more than INSTR_SET_SIZE(" << INSTR_SET_SIZE << ") instructions" << std::endl;
            return Answers{0, 0};
        }


        //init grid
        size_t index = GRID_LIN_SIZE*GRID_LIN_SIZE;
        while(index!=0){
            grid[index] = 0;
            index--;
        }
        Answers ans = Answers{0, 0};

        part1(grid, GRID_LIN_SIZE, ins_set, set_num);
        ans.answer1 = count_on(grid, GRID_LIN_SIZE);

        //reset grid
        index = GRID_LIN_SIZE*GRID_LIN_SIZE;
        while(index!=0){
            grid[index] = 0;
            index--;
        }

        part2(grid, GRID_LIN_SIZE, ins_set, set_num);
        ans.answer2 = get_total(grid, GRID_LIN_SIZE);

        delete[] grid;
        return ans;
    }

    void write_answers(){
        Answers res = run();

        std::cout << "\tLights On, part 1 : "<< res.answer1 << std::endl;
        std::cout << "\tLights On, part 2 : "<< res.answer2 << std::endl;
    }

}