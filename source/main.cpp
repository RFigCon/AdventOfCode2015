#include "global_in.hpp"

#include "day1/floors.cpp"
#include "day2/presents.cpp"
#include "day3/route.cpp"
#include "day4/hash.cpp"
#include "day5/nice_string.cpp"
#include "day6/lights.cpp"
#include "day7/circuit.cpp"

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

    std::cout << "\n----------------------------\n\tDay 6\n----------------------------\n";
    Lights::write_answers();

    std::cout << "\n----------------------------\n\tDay 7\n----------------------------\n";
    Circuit::write_answers();
    return 0;
}