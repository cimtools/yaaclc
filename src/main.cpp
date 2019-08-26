#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>

#include "../lib/Analizer_ACL.hpp"

using namespace std;

int main(){
    cout << "begining" << endl;
    Analizer_ACL analizer("./examples/base.dnl");
    cout << "Object created" << endl;

    cout << analizer.token_vector.size();
    analizer.lexer();

    return 0;
}