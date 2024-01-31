#ifndef Database_h
#define Database_h

#include <fstream>
#include <string.h>
#include <filesystem>
#include "Person.h"
#include "Index.h"

using namespace std;

class Database
{
private:
    Person *root = NULL;
    Index *ageIndexRoot = NULL;
    Index *nameIndexRoot = NULL;
    
    void updateIndex(string name)
    {
        Index *prev = NULL;
        for(Index *i = nameIndexRoot; i != NULL; i = i->next)
        {
            if(i->person->name == name) {
                if(prev == NULL) {
                    nameIndexRoot = i->next;
                } else {
                    prev->next = i->next;
                }
                break;
            }
            prev = i;
        }
        prev = NULL;
        for(Index *i = ageIndexRoot; i != NULL; i = i->next)
        {
            if(i->person->name == name) {
                if(prev == NULL) {
                    ageIndexRoot = i->next;
                } else {
                    prev->next = i->next;
                }
                break;
            }
            prev = i;
        }
    }
public:
    void add(string name, int age)
    {
        Person *p = new Person;
        p->name = name;
        p->age = age;
        p->next = NULL;
        
        Index *a = new Index;
        a->person = p;
        a->next = NULL;
        
        Index *n = new Index;
        n->person = p;
        n->next = NULL;
        
        if(root == NULL)
        {
            root = p;
            ageIndexRoot = a;
            nameIndexRoot = n;
        }
        else
        {
            // data
            for(Person *current = root; current != NULL; current = current->next)
            {
                if(current->next == NULL)
                {
                    p->prev = current;
                    current->next = p;
                    break;
                }
            }
            
            // index by age
            Index *prevA = ageIndexRoot;
            bool found = false;
            for(Index *i = ageIndexRoot; i != NULL; i = i->next)
            {
                if(a->person->age > i->person->age) {
                    prevA = i;
                    continue;
                } else {
                    //if(prev == ageIndex) {
                    if(a->person->age < prevA->person->age) {
                        a->next = ageIndexRoot;
                        ageIndexRoot = a;
                        found = true;
                    } else {
                        a->next = i;
                        prevA->next = a;
                        found = true;
                    }
                    prevA = i;
                    break;
                }
            }
            if(!found)
                prevA->next = a;
            
            // index by name
            Index *prevN = nameIndexRoot;
            found = false;
            for(Index *i = nameIndexRoot; i != NULL; i = i->next)
            {
                if(n->person->name > i->person->name) {
                    prevN = i;
                    continue;
                } else {
                    if(n->person->name < prevN->person->name) {
                        n->next = nameIndexRoot;
                        nameIndexRoot = n;
                        found = true;
                    } else {
                        n->next = i;
                        prevN->next = n;
                        found = true;
                    }
                    prevN = i;
                    break;
                }
            }
            if(!found)
                prevN->next = n;
        }
    }
    bool remove(string name)
    {
        bool found = false;
        for(Person *i = root; i != NULL; i = i->next) {
            if(i->name == name) {
                found = true;
                if(i->prev == NULL) {
                    root = i->next;
                } else {
                    i->prev->next = i->next;
                }
                updateIndex(name);
                break;
            }
        }
        return found;
    }
    void update(string name, int age)
    {
        for(Person *i = root; i != NULL; i = i->next)
        {
            if(i->name == name) {
                i->age = age;
                
                Person *rootTmp = root;
                //Index *ageIndexRootTmp = ageIndexRoot;
                //Index *nameIndexRootTmp = nameIndexRoot;
                
                root = NULL;
                ageIndexRoot = NULL;
                nameIndexRoot = NULL;
                
                for(Person *i = rootTmp; i != NULL; i = i->next) {
                    add(i->name, i->age);
                }
                
                break;
            }
        }
    }
    void printByAge()
    {
        for(Index *i = ageIndexRoot; i != NULL; i = i->next)
        {
            cout<<i->person->name<<" "<<i->person->age<<endl;
        }
    }
    void printByName()
    {
        for(Index *i = nameIndexRoot; i != NULL; i = i->next)
        {
            cout<<i->person->name<<" "<<i->person->age<<endl;
        }
    }
    void saveToFileByAge(string filename)
    {
        ofstream f;
        f.open(filename);
        if(!f.is_open()){
            cerr << "Error opening the file: " <<filename<<endl;
            return;
        }

        filesystem::path inputFilePath(filename);
        string directoryPath = inputFilePath.parent_path().string();
        string outputFileName = directoryPath + "/age.txt";

        for(Index *i = ageIndexRoot; i != NULL; i = i->next)
        {
            f<<i->person->name<<" "<<i->person->age<<endl;
        }
        f.close();
    }
    void saveToFileByName(string filename)
    {
        ofstream f;
        f.open(filename);
        for(Index *i = nameIndexRoot; i != NULL; i = i->next)
        {
            f<<i->person->name<<" "<<i->person->age<<endl;
        }
        f.close();
    }
    void loadFile(string filename)
    {
        ifstream file;
        file.open(filename.c_str());
        string n; // name
        int a; // age
        
        while (file >> n)
        {
            file >> a;
            add(n, a);
        }
        file.close();
    }
};

#endif /* Database_h */
