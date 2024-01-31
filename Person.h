#ifndef Person_h
#define Person_h

#include <string>

using namespace std;

class Person
{
public:
    string name;
    int age;
    Person *next, *prev;

};

#endif /* Person_h */
