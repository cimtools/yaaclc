#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>

#include "../lib/Analizer_ACL.hpp"

using namespace std;

int main(int argc, char* argv[]){
    cout << "begining" << endl;
    cout << argv[1] << endl;
    Analizer_ACL analizer(argv[1]);
    cout << "Object created" << endl;

    cout << analizer.token_vector.size();
    analizer.lexer();

    return 0;
}