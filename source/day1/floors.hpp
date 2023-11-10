#include "../global_in.hpp"

static const std::string RES_DAY1 = "resources/day1/";

class Floors{
private:
    int floor = 0;
    int entered_basement = 0;

    static Floors* get_floor_and_basement(std::string file_name){
        
        std::ifstream file;
        file.open(RES_DAY1 + file_name);

        Floors* result = new Floors();

        if(!file.is_open()) return result;

        int step = 0;
        bool visited_basement = false;
        char instruction;
        while(file.get(instruction)){
            
            switch(instruction){
                case '(':   
                    result->floor++;
                    step++;
                    break;
                case ')':
                    result->floor--;
                    step++;
                    break;
                default:
                    break;
            }

            if(!visited_basement && result->floor<0){
                visited_basement = true;
                result->entered_basement = step;    
            }

        }

        file.close();

        return result;

    }

public:
    static void write_answers(){
        Floors *floors = get_floor_and_basement("floor.txt");
        if(!floors) return;

        std::cout << "\t- Destination: floor " << floors->floor;
        std::cout << "\n\t- Basement at step " << floors->entered_basement;
        delete floors;
    }
};
