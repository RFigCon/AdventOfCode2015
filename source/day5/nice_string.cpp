#ifndef _GLOBAL_INC
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
    bool twice_flag;
    bool vowels_flag;
    bool valid_str_flag;
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

static unsigned int nice_str_in(std::string file_name){
    std::ifstream file(RES_DAY5 + file_name);

    if(!file.is_open()){
        return 0;
    }

    unsigned int old_rule_count = 0;
    symbs smb;
    flags fl;
    while( !file.eof() ){
        smb.cc = 0;
        smb.lc = 0;
        int vowel_count = 0;

        fl.twice_flag = false;
        fl.vowels_flag = false;
        fl.valid_str_flag = true;

        const int TRASH_SIZE = 30;
        char trash[TRASH_SIZE];

        do{
            smb.cc = file.get();
            if(smb.cc == '\n') break;

            if(!old_rules(smb, vowel_count, fl)){
                if(!file.eof()){
                    file.getline(trash, TRASH_SIZE);
                }
                break;
            }

            //TODO: Count nice string with new rules

        }while(!file.eof());

        if(fl.valid_str_flag && fl.twice_flag && fl.vowels_flag){
            old_rule_count++;
        }
        
    }

    return old_rule_count;
}

namespace NiceStr{
    void write_answers(){
        std::cout << "\t- There are " << nice_str_in("strings.txt") << " nice strings!" << std::endl;
    }
}