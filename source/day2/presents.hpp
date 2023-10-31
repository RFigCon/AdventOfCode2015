#ifndef _GLOBAL_INC
#include <fstream>
#include <string>
#include <iostream>
#endif

namespace{
    const std::string RES = "resources/day2/";
}
class Presents{
    
private:
    int paper_total = 0;
    int ribbon_total = 0;

    static const char DELIM = 'x';

    static int find_end(std::string str, int start_idx){

        char c = str[start_idx];
        while(c!=DELIM && c!='\0'){
            c = str[++start_idx];
        }
        
        return start_idx;
    }
    
    static int parse_int(std::string str, int start_idx, int end_idx){
        int val = 0;
        for(int i = start_idx; i< end_idx; i++){
            val = val*10;
            val += str[i] - '0';
        }
        return val;
    }

    static int get_smallest(int first, int second, int third){
        if(first<=second && first<=third) return first;
        if(second<=first && second<=third) return second;
        return third;
    }

    static Presents* get_total(std::string file_name){
        std::ifstream file (RES + file_name);

        if(!file.is_open()) return 0;

        Presents* prst = new Presents();

        const int LINE_SIZE = 30;
        char line[LINE_SIZE];

        int len, wid, hgh;
        int area1, area2, area3;

        while( file.getline(line, LINE_SIZE)) {

            int int_start = 0;
            int int_end = find_end(line, int_start);
            len = parse_int(line, int_start, int_end);

            int_start = int_end + 1;
            int_end = find_end(line, int_start);
            wid = parse_int(line, int_start, int_end);

            int_start = int_end + 1;
            int_end = find_end(line, int_start);
            hgh = parse_int(line, int_start, int_end);

            area1 = len*wid;
            area2 = wid*hgh;
            area3 = hgh*len;

            int smallest = get_smallest(area1, area2, area3);
            prst->paper_total += 2*(len*wid + wid*hgh + hgh*len) + smallest;

            //Add smallest perimeter to ribbon; in a rectangular/square shape, smaller area = smaller perimeter
            if(smallest == area1)
                prst->ribbon_total += 2*(len + wid);
            else if(smallest == area2)
                prst->ribbon_total += 2*(wid + hgh);
            else
                prst->ribbon_total += 2*(hgh + len);

            //Add volume to ribbon
            prst->ribbon_total += len*wid*hgh;

        }

        return prst;
    }

public:
    static void write_answers(){
        Presents* presents = get_total("dimensions.txt");
        if(!presents) return;

        std::cout << "\t- Total square feet of paper: " << presents->paper_total << "\n\t- Feet of ribbon: " << presents->ribbon_total;
        delete presents;
    }
};