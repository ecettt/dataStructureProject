#include <iostream>

#include "Database.h"

using namespace std;

int main(int argc, const char * argv[]) {

    Database d;
    
    d.loadFile("input.txt");
    
    d.printByName();
    d.printByAge();
    d.saveToFileByAge("input.txt");
    //d.saveToFileByName("input.txt");
    
    return 0;
}
