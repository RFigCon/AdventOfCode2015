#include <fstream>
#include <string>
#include <iostream>
using namespace std;

#include "day1/floors.hpp"
#include "day2/presents.hpp"
#include "day3/route.hpp"

int main(){
    cout << "----------------------------\n\tDay 1\n----------------------------\n";
    Floors::write_answers();

    cout << "\n----------------------------\n\tDay 2\n----------------------------\n";
    Presents::write_answers();

    cout << "\n----------------------------\n\tDay 3\n----------------------------\n";
    Route::write_answers();
    return 0;
}