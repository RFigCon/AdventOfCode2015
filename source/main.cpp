#include <fstream>
#include <string>
#include <iostream>
using namespace std;

#include "day1/floors.hpp"
#include "day2/presents.hpp"
#include "day3/route.hpp"
#include "day4/hash.cpp"

int main(){
    cout << "----------------------------\n\tDay 1\n----------------------------\n";
    Floors::write_answers();

    cout << "\n----------------------------\n\tDay 2\n----------------------------\n";
    Presents::write_answers();

    cout << "\n----------------------------\n\tDay 3\n----------------------------\n";
    Route::write_answers();

    cout << "\n----------------------------\n\tDay 4\n----------------------------\n";
    MD5::write_answers();
    return 0;
}