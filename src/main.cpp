#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

// const var lita de comandos
// var lista de variaveis
// var lista de rotinas
// var liked list de vector< variaveis de cada escopo > //COMO LIDA COM A ENTRADA DE UM PROGRAMA

inline bool is_letter( char c )__attribute__((always_inline));
inline bool is_letter( char c ){
    return ( ( c >= 65 ) && (c <= 90 ) ) || ( ( c >= 97) && ( c <= 122 ) ) ;
}

inline bool is_number( char c )__attribute__((always_inline));
inline bool is_number( char c ){
    return c >= 48 && c <= 57 ;
}

inline bool is_operator( char c )__attribute__((always_inline));
inline bool is_operator( char c ){
    return c == '<' || c == '>' || c == '=' || c == '&' || c == '+' || c == '-' || c == '/' || c == '.';
}
inline bool is_allowed_in_string( char c )__attribute__((always_inline));
inline bool is_allowed_in_string( char c ){
    return c >= 32 && c <= 126 && c != 34;
}

const set<string> commands = {"dimg" , "program", "dodin", "dim", "define", "for", "set", "end", "global"};

vector<string> get_token( ifstream * myfile ){
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
        }while( is_allowed_in_string(myfile->peek()) );
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

int main(){
    ifstream myfile ("prog.dnl");
    string line;
    if (myfile.is_open()){
        vector<string> token_readed = get_token(&myfile); 
        do{
            if(token_readed.at(1) != "NEW LINE" )
            cout << token_readed.at(0)<<endl;
            token_readed = get_token(&myfile);
        }while(token_readed.at(1)!="END");
        myfile.close();
    }
    else cout << "Unable to open file"; 

    return 0;
}