#include <iostream>
#include <string>
#include "dbms.hpp"


int main() {
    using std::endl;
    using std::cout;
    using std::cin;

    char input;
    do {
        cout << "\nType what you want to do! <s> select database, <c> create database\n";
        cin >> input;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (input)
        {
        case 's':
            cout << "select\n";
            break;
        case 'c':
            cout << "create\n";
            
            if (dbms::createNewDatabase("testdb")) {
                cout << "dbb created successfully";
            }
            else {
                cout << "error creating db!";
            }
            break;
        default:
            cout << "something different!\n";
            break;
        }
    } while (input != 'q');
    
    
    return 0;
}
