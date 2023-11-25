#include "../global_in.hpp"

static const std::string RES_DAY8 = "resources/day8/input.txt";

namespace EscapeChars{
    
    //Part 1
    static unsigned int get_diff_in_line(std::ifstream &file){
        
        unsigned int total = 0;
        unsigned int in_mem = 0;
        
        char c;
        do{
            c = file.get();
            if(c=='\n') break;
            
            total += 1;
            
            if(c == '"') continue;
            
            in_mem += 1;
            
            if(c!='\\') continue;
            
            c = file.get();
            total += 1;
            if(c != 'x') continue;
            
            c = file.get();
            c = file.get();
            total += 2;
        }while( !file.eof() );
        
        return total - in_mem;
    }
    
    static unsigned int get_total_diff_in_file(std::string &file_name){
        std::ifstream file(file_name);
        
        if( !file.is_open() ){
            std::cout << "Could not open file!" << std::endl;
            return 0;
        }
        
        unsigned int total_diff = 0;
        
        while( !file.eof() ){
            total_diff += get_diff_in_line(file);
        }
        
        file.close();
        return total_diff;
    }
    
    //Part 2
    static unsigned int get_new_line_size_diff(std::ifstream &file){
        if( file.peek() == EOF) return 0;
        
        unsigned int new_size = 2;
        unsigned int original_size = 0;
        
        char c;
        do{
            c = file.get();
            if(c=='\n') break;
            
            original_size += 1;
            new_size += 1;
            
            if(c == '"' || c=='\\') new_size+=1;
        }while( !file.eof() );
        
        return new_size - original_size;
    }
    
    static unsigned int get_total_diff_in_new_file(std::string &file_name){
        std::ifstream file(file_name);
        
        if( !file.is_open() ){
            std::cout << "Could not open file!" << std::endl;
            return 0;
        }
        
        unsigned int total_diff = 0;
        
        while( !file.eof() ){
            total_diff += get_new_line_size_diff(file);
        }
        
        file.close();
        return total_diff;
    }
    
    void write_answers(){
        std::string file_name(RES_DAY8);
        unsigned int total_diff1 = get_total_diff_in_file( file_name );
        unsigned int total_diff2 = get_total_diff_in_new_file( file_name );
        
        std::cout << "\tTotal difference (part 1): " << total_diff1 << std::endl;
        std::cout << "\tTotal difference (part 2): " << total_diff2 << std::endl;
    }
    
};