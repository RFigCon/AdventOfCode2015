#define _GLOBAL_INC
#include <fstream>
#include <string>
#include <iostream>

#include "day1/floors.hpp"
#include "day2/presents.hpp"
#include "day3/route.hpp"
#include "day4/hash.cpp"
#include "day5/nice_string.cpp"

int main(){
    std::cout << "----------------------------\n\tDay 1\n----------------------------\n";
    Floors::write_answers();

    std::cout << "\n----------------------------\n\tDay 2\n----------------------------\n";
    Presents::write_answers();

    std::cout << "\n----------------------------\n\tDay 3\n----------------------------\n";
    Route::write_answers();

    std::cout << "\n----------------------------\n\tDay 4\n----------------------------\n";
    //MD5::write_answers();

    std::cout << "\n----------------------------\n\tDay 5\n----------------------------\n";
    NiceStr::write_answers();
    return 0;
}