#include <iostream>
#include <string>
#include "db.hpp"

int main() {
    using std::endl;
    using std::cout;
    using std::cin;

    std::string str;
    cout << "Please enter what string you want to print out!" << endl;
    cin >> str;
    nosql::print(str);
    return 0;
}
