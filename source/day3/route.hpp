#include <set>

static const string RES_ROUTE = "resources\\day3\\";

class Route{

private:
    static int processBySanta(string file_name){
        
        int x_coor = 0;
        int y_coor = 0;
        std::set< std::pair<int, int> > places;

        ifstream file;
        file.open(RES_ROUTE + file_name);

        if(!file.is_open()) return 0;

        int counter = 1;
        places.insert({x_coor, y_coor});

        char step;
        while(file.get(step)){
            switch(step){
                case '<':
                    x_coor--;
                    break;
                case '>':
                    x_coor++;
                    break;
                case '^':
                    y_coor++;
                    break;
                case 'v':
                    y_coor--;
                    break;
                default:
                    break;
            }
            
            if( places.insert({x_coor, y_coor}).second == true ){
                counter++;
            }
        }

        return counter;
        
    }



    //-----------Part 2------------

    struct Coor{
        int x;
        int y;
    };

    static void move(Coor *obj, char dir){
        switch(dir){
            case '<':
                obj->x--;
                break;
            case '>':
                obj->x++;
                break;
            case '^':
                obj->y++;
                break;
            case 'v':
                obj->y--;
                break;
            default:
                break;
        }
    }

    static int processWithRobot(string file_name){
        
        Coor santa = {0, 0};
        Coor robo = {0, 0};
        
        std::set< std::pair<int, int> > places;

        ifstream file;
        file.open(RES_ROUTE + file_name);

        if(!file.is_open()) return 0;

        int counter = 1;
        places.insert({0, 0});

        char step;
        bool santa_turn = true;
        while(file.get(step)){
            
            if(santa_turn){
                move(&santa, step);
                if( places.insert({santa.x, santa.y}).second == true ){
                    counter++;
                }
            }else{
                move(&robo, step);
                if( places.insert({robo.x, robo.y}).second == true ){
                    counter++;
                }
            }
            
            santa_turn = !santa_turn;
        }

        return counter;
        
    }

public:
    static void write_answers(){
        int count = processBySanta("directions.txt");
        cout << "\t- Number of houses visited (only Santa): " << count << endl;

        count = processWithRobot("directions.txt");
        cout << "\t- Number of houses visited (with Robot): " << count << endl;
    }

};