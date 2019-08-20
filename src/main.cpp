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
struct Token{
    string content;
    string type;
};

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
    bool check_token_sequence( vector<string> sentence );
    int get_tokens();
    void lexer();

    list<map<string,string>> scopes;
    list<map<string,string>>::iterator scope_iterator;
    vector<Token *> token_vector;
    vector<Token *>::iterator token_iterator;
    int scope=0;
    ifstream * myfile;

};

Analizer_ACL::Analizer_ACL(){
    myfile = new ifstream( "prog.dnl" );
    map<string,string> global_scope;
    scopes.push_back(global_scope);
    scope_iterator = scopes.begin();
    get_tokens();
}

bool Analizer_ACL::check_token_sequence( vector<string> type_sequence ){
    vector<Token *>::iterator entry_state = token_iterator;
    if(equal( type_sequence.begin(), type_sequence.end(), token_iterator, [](string first, Token * second){
        if( first.at(0) == '_'){
            first.erase(0,1);
            return second->content == first;
        }else{
            return second->type == first;
        }
    })){
        ++token_iterator;//incresed by 1 to say that all this token, including the las, was processed.
        return true;
    }else{
        token_iterator = entry_state;
        return false;
    }
}

/*
 *  @brief
 *  @param ponteiro do tipo ifstream que aponta para o arquivo myfile 
 */
int Analizer_ACL::get_tokens(){
    
    //Token * readed_token = new Token();
    // readed_token->content = "";
    // readed_token->type = "";
    string token;
    char c = myfile->peek();

    while( c == 42 ){ 

        do{
           myfile->get();
        }while( myfile->peek() != '\n' );
        c = myfile->peek();
    }

    if( myfile->is_open() ){
        do{
            Token * readed_token = new Token();
            token="";

            c = myfile->peek();

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

                readed_token->content = token;

                if( commands.find(token) != commands.end() ){
                    readed_token->type ="COMMAND";
                }else{
                    readed_token->type ="WORD";
                }

            }else if( is_number(c) ){ 
                do{
                    token += myfile->get();
                }while( is_number(myfile->peek()) );

                readed_token->content = token;
                readed_token->type = "NUMBER";

            }else if( is_operator(c) ){ 
                do{
                    token += myfile->get();
                }while( is_operator(myfile->peek()) );

                readed_token->content = token;
                readed_token->type = "OPERATOR";

            }else if( c == '"' ){
                do{
                    token += myfile->get();
                }while( is_allowed_in_string(myfile->peek() ) && token.size()<= 40 );
                if( myfile->peek()!= '"' ){
                    //THROW ERROR
                    cout << "sintax error!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
                    token = myfile->peek();
                    readed_token->content = token;
                    readed_token->type = "ERROR";     
                    return -1;       
                }else{
                    token+=myfile->get();
                }

                readed_token->content = token;
                readed_token->type = "STRING";

            }else if( c == '[' ){
                do{
                    token += myfile->get();
                }while( is_number(myfile->peek()));
                if( myfile->peek() != ']'){
                    //THROW ERROR
                    cout << "sintax error!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
                    token = myfile->peek();
                    readed_token->content = token;
                    readed_token->type = "ERROR";
                    return -2;
                }else{
                    token += myfile->get();
                }

                readed_token->content = token;
                readed_token->type = "INDEX";

            }else if( c == '\n'  ){
                do{
                    token += myfile->get();
                }while( myfile->peek() == '\n' );

                readed_token->content = token;
                readed_token->type ="NEW LINE";

            }else if( c == EOF){
                readed_token->content = "";        
                readed_token->type ="END";

            }else if( c == ' ' || c == '\t' ){
                do{
                    token += myfile->get();
                    c = myfile->peek();
                }while( c==' ' || c == '\t' );

                readed_token->content = token;
                readed_token->type ="WHITE SPACE";

            }else{
                cout<< "None of the above!!     :  " << (int)c ;

                token = c;
                readed_token->content = token;
                readed_token->type ="ERROR";

            }   
            token_vector.push_back(readed_token);
        }while( token!="" );
    }    
    return 1; 
}

void Analizer_ACL::lexer(){
    cout<< "Initializing lexer\n"<< "token_vector size is:  "<<token_vector.size() <<"\n first token is:  "<< (token_vector.at(3))->type<<endl;
    token_iterator=token_vector.begin();
    do{
        cout << (*token_iterator)->type << endl;
        if ( check_token_sequence({"WHITE SPACE"})){
        }else if( check_token_sequence({"NEW LINE"})){
        }else if( check_token_sequence({"_dimg","WHITE SPACE","WORD","INDEX", "NEW LINE"})){
            string index = (*(token_iterator-1))->content;
            index.erase(0,1);
            index.pop_back();
            for(int i =0; i < atoi(index.c_str()); i++ ){
                cout << "\nDeclaring global variable named   " << "var_" + (*(token_iterator-3))->content +"[" + to_string(i) + "]"<<endl;
                (*scopes.begin())[ "var_" + (*(token_iterator-3))->content +"[" + to_string(i) + "]" ] = "";
            }
        }else if( check_token_sequence({"_if", "WHITE SPACE", "WORD", "INDEX", "WHITE SPACE", "OPERATOR", "WHITE SPACE", "WORD", "INDEX" })){
            if( (*scope_iterator)["var_" + (*(token_iterator-7))->content + (*(token_iterator-6))->content] == (*scope_iterator)["var_" + (*(token_iterator-2))->content + (*(token_iterator-1))->content]){
                //entrar no escopo do if
                map<string,string> new_scope;
                scopes.push_back(new_scope);
                ++scope_iterator;
            }else{
                do{
                    ++token_iterator;
                    if((*token_iterator)->type == "END"){
                        cout << "coudn`t find the end of if block\n";
                        break;
                    }
                }while( (*token_iterator)->content != "else" && (*token_iterator)->content != "endif" );
            }
                
        }else if( check_token_sequence({"_else"}) ){
            do{
                ++token_iterator;
                if((*token_iterator)->type == "END"){
                    cout << "coudn`t find the end of if block\n";
                    break;
                }
            }while( (*token_iterator)->content != "endif");
        }else if((*token_iterator)->type == "END"){
            break;
        }
        
    }while(false);

}

int main(){
    Analizer_ACL analizer;
    Token leitura;
    
    analizer.lexer();
    return 0;
}