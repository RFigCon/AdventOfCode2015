#ifndef _GLOBAL_INC
#define _GLOBAL_INC
#include <fstream>
#include <string>
#include <iostream>
#endif

static const std::string RES_DAY5 = "resources/day5/";

struct symbs{
    char cc;
    char lc;
};

struct flags{
    //Part 1
    bool twice_flag;
    bool vowels_flag;
    bool valid_str_flag;

    //Part 2
    bool double_pair_flag;
    bool repeat_w_break_flag;
};

static bool old_rules(symbs &smb, int &vowel_count, flags &fl){

    switch (smb.cc) {
        case 'b':
        case 'd':
        case 'q':
        case 'y':
            if( (smb.cc-1) == smb.lc) fl.valid_str_flag = false;
        default:
            break;
    }

    if(!fl.valid_str_flag){
        return false;
    }

    if(vowel_count<3){
        switch (smb.cc) {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
                ++vowel_count;
                if(vowel_count == 3) fl.vowels_flag = true;
            default:
                break;
        }
    }
    
    if(smb.cc == smb.lc) fl.twice_flag = true;

    smb.lc = smb.cc;

    return true;
}

static void new_rules(char* str, int max_size, flags &fl){
    if(str[0] == '\0' || str[1] == '\0'){
        return;
    }

    for(int i = 0; i+2 < max_size; i++){

        if(str[i+2]=='\0'){
            return;
        }

        if(str[i] == str[i+2]){
            fl.repeat_w_break_flag = true;
            break;
        }
    }

    for(int i = 0; i+3 < max_size && str[i+3]!='\0'; i++){

        for(int j = i+2; j+1 < max_size && str[j+1]!='\0'; j++){

            if(str[i]==str[j] && str[i+1]==str[j+1]){
                fl.double_pair_flag = true;
                return;
            }
            
        }

    }

}

static unsigned int* nice_str_in(std::string file_name){
    std::ifstream file(RES_DAY5 + file_name);

    if(!file.is_open()){
        return 0;
    }

    static unsigned int count[2];
    int old_rule_count = 0;
    int new_rule_count = 1;
    count[old_rule_count] = 0;
    count[new_rule_count] = 0;

    const int LINE_SIZE = 30;
    char line[LINE_SIZE];

    symbs smb;
    flags fl;
    while( !file.eof() ){
        smb.cc = 0;
        smb.lc = 0;
        int vowel_count = 0;

        //Part 1 flags
        fl.twice_flag = false;
        fl.vowels_flag = false;
        fl.valid_str_flag = true;

        //Part 2 flags
        fl.double_pair_flag = false;
        fl.repeat_w_break_flag = false;

        int line_idx = 0;
        smb.cc = file.get();
        while(!file.eof() && smb.cc != '\n'){
            
            line[line_idx++] = smb.cc;

            if(!old_rules(smb, vowel_count, fl)){
                if(!file.eof()){
                    file.getline(&line[line_idx], LINE_SIZE-line_idx, '\n');
                }
                break;
            }
            smb.cc = file.get();
        }
        if(fl.valid_str_flag){  //if true, then the instruction "file.getline" was not ran
            line[line_idx++] = '\0';
        }

        new_rules(line, LINE_SIZE, fl);

        if(fl.valid_str_flag && fl.twice_flag && fl.vowels_flag){
            count[old_rule_count]++;
        }
        
        if(fl.double_pair_flag && fl.repeat_w_break_flag ){
            count[new_rule_count]++;
        }
        
    }

    file.close();
    return count;
}

namespace NiceStr{
    void write_answers(){
        unsigned int* ptr = nice_str_in("strings.txt");
        std::cout << "\t- There are " << ptr[0] << " nice strings, according to the old rules!" << std::endl;
        std::cout << "\t- There are " << ptr[1] << " nice strings, according to the new rules!" << std::endl;
    }
}