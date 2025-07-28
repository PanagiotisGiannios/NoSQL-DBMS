#include <iostream>
#include <string>
#include "dbms.hpp"


int main() {
    using std::endl;
    using std::cout;
    using std::cin;

    char input;
    std::string selection;

    do {
        cout << "\nType what you want to do! <s> select database, <c> create database\n";
        cin >> input >> selection;
        //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "You entered char: '" << input << "' and string: \"" << selection << "\"\n";
        switch (input)
        {
        case 's':
            cout << "select\n";
            if (dbms::selectDatabase(selection)) {
                cout << "selected database: testdb succesfully\n";
            }
            else {
                cout << "Database selection error:" << dbms::getErrorMessage() << "\n";
                dbms::clearError();
            }
            break;
        case 'c':
            cout << "create\n";
            
            if (dbms::createNewDatabase(selection)) {
                cout << "dbb created successfully";
            }
            else {
                cout << "Database creation error: " << dbms::getErrorMessage() << "\n";
                dbms::clearError();
            }
            break;
        default:
            cout << "something different!\n";
            break;
        }
    } while (input != 'q');
    
    
    return 0;
}
