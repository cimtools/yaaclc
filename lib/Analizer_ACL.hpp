#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>

using namespace std;

struct Token{
    string content;
    string type;
};

/* 
 *  @brief This block is responsible for the interpreter identify what is a letter.
 *  Through this block the interpreter can recognize if what was typed is a letter.
 *  @param char c is the parameter that will be tested by is_letter 
*/
inline bool is_letter( char c )__attribute__((always_inline));
inline bool is_letter( char c ){
    return ( ( c >= 65 ) && (c <= 90 ) ) || ( ( c >= 97) && ( c <= 122 ) ) ;
}

/* 
 *  @brief This block is responsible for the interpreter identify what is a number. Through
 *   this block the interpreter can recognize if what was typed is a number.
 *  @param char c is the parameter that will be tested by is_number.
 */
inline bool is_number( char c )__attribute__((always_inline));
inline bool is_number( char c ){
    return c >= 48 && c <= 57 ;
}

/* 
 *  @brief This block is responsible for the interpreter identify what is a operator 
 *  @param char c is the parameter that will be tested by is_operator
 */
inline bool is_operator( char c )__attribute__((always_inline));
inline bool is_operator( char c ){
    return c == '<' || c == '>' || c == '=' || c == '&' || c == '+' || c == '-' || c == '/' || c == '.';
}

/* 
 *  @brief This block is responsible to identify when a string is allowed. 
 *  @param char c is the parameter that will be tested by is_allowed_in_string  
 */
inline bool is_allowed_in_string( char c )__attribute__((always_inline));
inline bool is_allowed_in_string( char c ){
    return c >= 32 && c <= 126 && c != 34;
}

/*
 *  @brief const set<string> comands is our command list accept by our compiler
 */
const set<string> commands = { "dimg" , "if", "else", "endif", "set", "println" };

class Analizer_ACL{
public:
    Analizer_ACL();
    Analizer_ACL( string file_path );
    bool check_token_sequence( vector<string> sentence );
    int get_tokens();
    void lexer();

    list<map<string,string> * > scopes;
    list<map<string,string> * >::iterator scope_iterator;
    vector<Token *> token_vector;
    vector<Token *>::iterator token_iterator;
    int scope=0;
    ifstream * myfile;

};
