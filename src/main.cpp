#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

vector<string> get_token( ifstream * myfile ){
    vector<string> return_vector;
    string token;
    char c = myfile->get();

    if( is_letter(c) ){ 
        token += c;
        c = myfile->get();
        while( is_letter(c) || is_number(c) ){
            token += c;
            c = myfile->get();
        }
        myfile->unget();

        return_vector.push_back(token);
        return_vector.push_back("WORD");

    }else if( is_number(c) ){ 
        token += c;
        c = myfile->get();
        while( is_number(c) ){
            token += c;
            c = myfile->get();
        }
        myfile->unget();

        return_vector.push_back(token);
        return_vector.push_back("NUMBER");

    }else if( is_operator(c) ){ 
        token += c;
        c = myfile->get();
        while( is_operator(c) ){
            token += c;
            c = myfile->get();
        }
        myfile->unget();

        return_vector.push_back(token);
        return_vector.push_back("OPERATOR");

    }else if( c == '"' ){
        token += c;
        c = myfile->get();
        while( (c >= 35 && c <= 126) || c == 33 || c == 32 ){
            token += c;
            c = myfile->get();
        }
        if( c!= '"' ){
            //THROW ERROR
            cout << "sintax error!!!!!!!!!!!!!!!!!!!!!!!!!!" << (int)c << "  " << token << endl;
            token = c;
            return_vector.push_back(token);
            return_vector.push_back("ERROR");            
        }
        token += c;

        return_vector.push_back(token);
        return_vector.push_back("STRING");

    }else if( c == '[' ){
        token += c;
        c = myfile->get();
        while( is_number(c) ){
            token += c;
            c = myfile->get();
        }
        if( c != ']'){
            //THROW ERROR
            cout << "sintax error!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            token = c;
            return_vector.push_back(token);
            return_vector.push_back("ERROR");
        }
        token += c;

        return_vector.push_back(token);
        return_vector.push_back("INDEX");

    }else if( c == '\n' || c == 42){
        token += c;
        while( c == '\n' || c == 42){
            token += c;
            c = myfile->get();
        }
        myfile->unget();

        return_vector.push_back(token);
        return_vector.push_back("NEW LINE");

    }else if( c == EOF){
        token += c;
        return_vector.push_back(token);
        return_vector.push_back("END");

    }else if( c == ' ' || c == '\t' ){
        token += c;
        while( c==' ' || c == '\t' ){
            token += c;
            c = myfile->get();
        }
        myfile->unget();

        return_vector.push_back(token);
        return_vector.push_back("White Space");

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
    
    if (myfile.is_open()){
       vector<string> token_readed = get_token(&myfile); 
        while(token_readed.at(1) != "END"){
            cout << token_readed.at(0) << endl;
            token_readed = get_token(&myfile);
        }
        myfile.close();
    }
    else cout << "Unable to open file"; 

    return 0;
}