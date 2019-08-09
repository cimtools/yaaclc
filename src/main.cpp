#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>

using namespace std;

// const var lita de comandos
// var lista de variaveis
// var lista de rotinas
// var liked list de vector< variaveis de cada escopo > //COMO LIDA COM A ENTRADA DE UM PROGRAMA

/* 
 *  @brief Bloco responsável para o interpretador entender o que é uma letra 
 *  @param char c é o parâmetro que será testado pelo is_letter 
*/
inline bool is_letter( char c )__attribute__((always_inline));
inline bool is_letter( char c ){
    return ( ( c >= 65 ) && (c <= 90 ) ) || ( ( c >= 97) && ( c <= 122 ) ) ;
}

/* 
 *  @brief Bloco responsável para o interpretador entender o que é um número, ou seja, através dele
 *  o identificador consegue reconhecer que o que foi digitado é, de fato, um número. .
 *  @param char c é o parâmetro que será testado pelo is_number.
 */
inline bool is_number( char c )__attribute__((always_inline));
inline bool is_number( char c ){
    return c >= 48 && c <= 57 ;
}

/* 
 *  @brief Bloco responsável para o interpretador entender o que é um operador 
 *  @param char c é o parâmetro que será testao pelo is_operator
 */
inline bool is_operator( char c )__attribute__((always_inline));
inline bool is_operator( char c ){
    return c == '<' || c == '>' || c == '=' || c == '&' || c == '+' || c == '-' || c == '/' || c == '.';
}

/* 
 *  @brief Bloco responsável para identificar quando uma string é permitida
 *  @param char c é o parâmetro que será testado pelo is_allowed_in_string  
 */
inline bool is_allowed_in_string( char c )__attribute__((always_inline));
inline bool is_allowed_in_string( char c ){
    return c >= 32 && c <= 126 && c != 34;
}
/*
 *  @brief Lista de comandos aceitos pelo interpretador
 */
const set<string> commands = {"dimg" , "program", "dodin", "dim", "define", "for", "set", "end", "global"};

class Analizer_ACL{
public:
    Analizer_ACL();
    //list<map<string,string>> scopes;
    //vector<string,string> token_vector;
    vector<string> get_token();

    ifstream * myfile;

};

Analizer_ACL::Analizer_ACL(){
    myfile = new ifstream( "prog.dnl" );
    //myfile->open;
}

/*
 *  @brief
 *  @param ponteiro do tipo ifstream que aponta para o arquivo myfile 
 */
vector<string> Analizer_ACL::get_token(){
    vector<string> return_vector;
    string token;
    char c = myfile->peek();

    while( c == 42 ){ 

        do{
           myfile->get();
        }while( myfile->peek() != '\n' );
        c = myfile->peek();
    }

    if( is_letter(c) ){ 
        do{
            token += myfile->get();
            c = myfile->peek();
        }while( is_letter(c) || is_number(c) );

        return_vector.push_back(token);

        if( commands.find(token) != commands.end() ){
            return_vector.push_back("COMMAND");
        }else{
            return_vector.push_back("WORD");
        }

    }else if( is_number(c) ){ 
        do{
            token += myfile->get();
        }while( is_number(myfile->peek()) );

        return_vector.push_back(token);
        return_vector.push_back("NUMBER");

    }else if( is_operator(c) ){ 
        do{
            token += myfile->get();
        }while( is_operator(myfile->peek()) );

        return_vector.push_back(token);
        return_vector.push_back("OPERATOR");

    }else if( c == '"' ){
        do{
            token += myfile->get();
        }while( is_allowed_in_string(myfile->peek() ) && token.size()<= 40 );
        if( myfile->peek()!= '"' ){
            //THROW ERROR
            cout << "sintax error!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            token = myfile->peek();
            return_vector.push_back(token);
            return_vector.push_back("ERROR");     
            return return_vector;       
        }else{
            token+=myfile->get();
        }

        return_vector.push_back(token);
        return_vector.push_back("STRING");

    }else if( c == '[' ){
        do{
            token += myfile->get();
        }while( is_number(myfile->peek()));
        if( myfile->peek() != ']'){
            //THROW ERROR
            cout << "sintax error!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            token = myfile->peek();
            return_vector.push_back(token);
            return_vector.push_back("ERROR");
            return return_vector;
        }else{
            token += myfile->get();
        }

        return_vector.push_back(token);
        return_vector.push_back("INDEX");

    }else if( c == '\n'  ){
        do{
            token += myfile->get();
        }while( myfile->peek() == '\n' );

        return_vector.push_back(token);
        return_vector.push_back("NEW LINE");

    }else if( c == EOF){
        return_vector.push_back("");
        return_vector.push_back("END");
        return return_vector;

    }else if( c == ' ' || c == '\t' ){
        do{
            token += myfile->get();
            c = myfile->peek();
        }while( c==' ' || c == '\t' );

        return_vector.push_back(token);
        return_vector.push_back("WHITE SPACE");

    }else{
        cout<< "None of the above!!     :  " << (int)c ;

        token = c;
        return_vector.push_back(token);
        return_vector.push_back("ERROR");

    }   

    return return_vector; 
}

// void lexer( vector<string> token ){
//     if( token.at(1) == "COMMAND" ){
//         if( token.at(0) == "dimg" ){
//         if( token.at(1) == "WHITE SPACE" ){     
//         if( token.at(1) == "WORD" ){

//         }
//         }
//         }
//     }
// }

int main(){
    Analizer_ACL analizer;
    vector<string> leitura;

    if( analizer.myfile->is_open() ){
        for( int i =0; i<300; ++i ){
            leitura = analizer.get_token();
            cout<< leitura[0];
        };
    }
    return 0;
}