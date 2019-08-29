#include "../lib/Analizer_ACL.hpp"
#include <unistd.h>
//#include <windows.h>
// #define DEBUGGING
// #define DEBUGGING2
using namespace std;

Analizer_ACL::Analizer_ACL(){
    myfile = new ifstream( "base.dnl" );
    map<string,string> * global_scope = new map<string,string>;
    scopes.push_back(global_scope);
    scope_iterator = scopes.begin();
    #ifdef DEBUGGING
    cout << "Constructor: start getting tokens" << endl;
    #endif
    get_tokens();
}

Analizer_ACL::Analizer_ACL( string file_path ){
    myfile = new ifstream( file_path );
    map<string,string> * global_scope = new map<string,string>;
    scopes.push_back(global_scope);
    scope_iterator = scopes.begin();
    #ifdef DEBUGGING
    cout << "Constructor: start getting tokens" << endl;
    #endif
    get_tokens();
}

bool Analizer_ACL::eat_tokens_if_match( vector<string> type_sequence ){
    vector<Token *>::iterator after_state = token_iterator;
    if(equal( type_sequence.begin(), type_sequence.end(), token_iterator, [&](string first, Token * second){
        ++after_state;
        if( first.at(0) == '_'){
            first.erase(0,1);
            return second->content == first;
        }else{
            return second->type == first;
        }
    })){
        token_iterator = after_state;//Eat the tokens.
        return true;
    }else{
        return false;
    }
}

string Analizer_ACL::search_scopes( string key ){
    string return_value = "__ERRCODE: Couldn't find variable name";

    //First chek if it is on global scope.
    if( (**(scopes.begin())).count(key) ){
        return_value = (**(scopes.begin()))[key];
    }else{
        for( auto search_iterator = scope_iterator; search_iterator != scopes.begin(); --search_iterator ){
            if( (**search_iterator).count(key) ){//If there is at least one occurence of the key on the current map.
                return_value = (**search_iterator)[key];
                break;//find the value, so break the loop.
            }
        }
    }
    if( return_value == "__ERRCODE: Couldn't find variable name\n"){
        cout << "Couldn't find the variable name.\n";
        return "__ERR";
    }

    return return_value;
}

string Analizer_ACL::get_relative_token_content( int relative_position ){
    string content;

    content = (*(token_iterator+relative_position))->content;
    #ifdef DEBUGGING
    cout << "get_relative_token_content:The readed token is " << content << endl;
    #endif
    return content;
}

string Analizer_ACL::get_relative_token_type( int relative_position ){
    string type;

    type = (*(token_iterator+relative_position))->type;
    #ifdef DEBUGGING
    cout << "get_relative_token_content:The readed token is " << type << endl;
    #endif
    return type;
}

void Analizer_ACL::skip_block(){
    int scope_level = 0;
    do{

        ++token_iterator; //start analizing the next token.
        #ifdef DEBUGGING2
        cout << "skip_block: Token being analized is   " << (*token_iterator)->content << "      From type        " << (*token_iterator)->type << "       scope_level has value     "<< scope_level  <<endl;
        #endif
        if((*token_iterator)->type == "END"){
            cout << "SKIPING IF BLOCK ERROR: coudn`t find the end of if block\n";
            break;
        }else if( (*token_iterator)->content == "if" ){
            ++scope_level;
        }else if( (*token_iterator)->content == "endif" ){
            --scope_level;
        }

    }while( !(  ( (*token_iterator)->content == "else" && scope_level == 0 )   ||  ( (*token_iterator)->content != "endif" && scope_level == -1 ) ) );
    ++token_iterator;//The token skiped was a else to run instead of an if, or it was the endif in case the if didn`t had an else.

    #ifdef DEBUGGING2
    cout << "skip_block: When exiting the function the current token is   " << (*token_iterator)->content << "      From type        " << (*token_iterator)->type << endl;
    #endif
    return;
}
/*
 *  @brief
 *  @param Type pointer ifstream that point to myfile file.
 */
int Analizer_ACL::get_tokens(){
    
    Token * readed_token;
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
            readed_token = new Token();
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
    }else{
        cout << "The file is not open." << endl;
    }    
    #ifdef DEBUGGING
    cout << "The number of tokens readed were " << token_vector.size() << endl;
    #endif
    return 1; 
}

void Analizer_ACL::lexer(){
    int line_count, iterations;

    line_count =0;
    iterations = 0;
    token_iterator=token_vector.begin();

    do{
        #ifdef DEBUGGING
        cout << "\n\nPosicaoao do iterador no comeco da analise  "<<std::distance(token_vector.begin(), token_iterator) << endl;
        cout<< "Start decoding token:    "<< (**token_iterator).content << " ------- " << (**token_iterator).type <<endl;
        #endif
        if ( eat_tokens_if_match({"WHITE SPACE"}) ){
            //Ignore.
        }
        else if( eat_tokens_if_match({"NEW LINE"}) ){

            ++line_count;

        }
        else if( eat_tokens_if_match({"_dimg","WHITE SPACE","WORD","INDEX", "NEW LINE"})){
            string map_key, index;
            int number_of_indexes;

            ++line_count;

            map_key = "var_" +  get_relative_token_content(-3) +"[";
            index =  get_relative_token_content(-2);
            index.erase(0,1); // remove '[' from index (first char)
            index.pop_back(); // remove ']' from index (last char)
            number_of_indexes = atoi(index.c_str());

            for(int i =0; i <= number_of_indexes; ++i ){
                #ifdef DEBUGGING
                cout << "\nDeclaring global variable named   " << map_key + to_string(i) + "]" << endl;
                #endif
                (**scopes.begin())[ map_key + to_string(i) + "]" ] = "";
            }

        }
        else if( eat_tokens_if_match({"_if", "WHITE SPACE"})){//Run the if block until an else, or jump until the correspondent else.
            string first_key, first_value, second_key, second_value, used_operator;
            bool condition;

            if( eat_tokens_if_match({"WORD","INDEX","WHITE SPACE"}) ){
                first_key = "var_" +  get_relative_token_content(-3) +  get_relative_token_content(-2);
            }else if( eat_tokens_if_match({"WORD", "WHITE SPACE"}) ){
                first_key = "var_" +  get_relative_token_content(-2);
            }else{
                cout << "\n!Expecting variable. Sintax error." << line_count << endl;
            }
            #ifdef DEBUGGING
            cout << "First key is  " << first_key << endl;
            #endif
            first_value = search_scopes( first_key );
            
            if( eat_tokens_if_match({"_<", "WHITE SPACE", "_>", "WHITE SPACE" })){
                used_operator = "< >";
            }else if( eat_tokens_if_match({ "OPERATOR", "WHITE SPACE" }) ){
                used_operator =  get_relative_token_content(-2);
            }else{
                cout << "\n!!Expecting operator. Sintax error." << line_count << endl;
            }

            if( eat_tokens_if_match({ "WORD", "INDEX", "NEW LINE" }) ){
                second_key = "var_" +  get_relative_token_content(-3) +  get_relative_token_content(-2);
                second_value = search_scopes( second_key );
            }else if( eat_tokens_if_match({ "WORD", "NEW LINE" }) ){
                second_key = "var_" +  get_relative_token_content(-2);
                second_value = search_scopes( second_key );
            }else if( eat_tokens_if_match({ "NUMBER", "NEW LINE" }) ){
                second_value =  get_relative_token_content(-2);
            }else{
                cout << "\n!Expecting variable. Sintax error." << line_count << endl;
            }

            ++line_count;
            #ifdef DEBUGGING
            cout << "The first value is " << first_value << "  the second value is " << second_value << endl;
            #endif
            if( used_operator == "=" ){
                condition = first_value == second_value;
            }else if( used_operator == "<"){
                condition = atoi(first_value.c_str()) < atoi(second_value.c_str()); 
            }else if( used_operator == ">"){
                condition = atoi(first_value.c_str()) > atoi(second_value.c_str());
            }else if( used_operator == "<=" ){
                condition = atoi(first_value.c_str()) <= atoi(second_value.c_str());
            }else if( used_operator == ">="){
                condition = atoi(first_value.c_str()) >= atoi(second_value.c_str());
            }else if( used_operator == "< >"){
                condition = atoi(first_value.c_str()) != atoi(second_value.c_str());
            }else{
                cout << "Error identifying the operator for comparison." << endl;
            }
            #ifdef DEBUGGING
            cout << "\nif condition is " << condition << endl;
            #endif
            if( condition ){
                //Enter if scope.
                scopes.push_back( new map<string,string> );
                ++scope_iterator;
            }else{
                //If case the condition wasent met, skip (the if block) until find endif or else. If get END token it is a ERROR!!
                skip_block();
            }
                
        }
        else if( eat_tokens_if_match({"_else"}) ){//If is reading else at this point is because it was inside an if, so the else block must be skipped.
            do{
                ++token_iterator;
                if((*token_iterator)->type == "END"){
                    cout << "coudn`t find the end of if block\n";
                    break;
                }
            }while( (*token_iterator)->content != "endif");
            ++token_iterator;//Skip the token that sinalizes the end of the block;

            --scope_iterator;
            scopes.pop_back();
        }
        else if( eat_tokens_if_match({ "_println", "WHITE SPACE", "STRING" })){ 
            string message, temp_read;

            message = get_relative_token_content(-1);
            message.erase(0,1); // erase the first char (")
            message.pop_back(); // erase the last char (")

            while( eat_tokens_if_match({ "WHITE SPACE", "STRING" })){
                temp_read = get_relative_token_content(-1);
                temp_read.erase(0,1);
                temp_read.pop_back();
                message += temp_read;
            }

            if( ! eat_tokens_if_match({ "NEW LINE" }) ){
                cout << "ERROR: EXPECTING NEW LINE TOKEN!!!!!!!" << endl;
            }
            #ifdef DEBUGGING
            cout << "_println case: Printing message   " << message << endl;
            #endif
            cout << message << endl;
        }
        else if( eat_tokens_if_match({ "_print", "WHITE SPACE", "STRING" })){ 
            string message, temp_read;

            message = get_relative_token_content(-1);
            message.erase(0,1); // erase the first char (")
            message.pop_back(); // erase the last char (")

            while( eat_tokens_if_match({ "WHITE SPACE", "STRING" })){
                temp_read = get_relative_token_content(-1);
                temp_read.erase(0,1);
                temp_read.pop_back();
                message += temp_read;
            }

            if( ! eat_tokens_if_match({ "NEW LINE" }) ){
                cout << "ERROR: EXPECTING NEW LINE TOKEN!!!!!!!" << endl;
            }else{
                ++line_count;
            }

            cout << message ;
        }
        else if( eat_tokens_if_match({ "_endif" }) ){    
            //If it hit an endif here it is geting out of a scope, then pop the scope
            --scope_iterator;
            scopes.pop_back();

        }
        else if( eat_tokens_if_match({ "_set", "WHITE SPACE", "WORD" }) ){
            string key, value;

            if( eat_tokens_if_match({ "INDEX", "WHITE SPACE", "_=", "WHITE SPACE", "NUMBER" }) ){
                key = "var_" + (*(token_iterator-6))->content + (*(token_iterator-5))->content;
                value = (*(token_iterator-1))->content;
            }else if( eat_tokens_if_match({ "WHITE SPACE", "_=", "WHITE SPACE", "NUMBER" }) ){
                key = "var_" + (*(token_iterator-5))->content;
                value = (*(token_iterator-1))->content;
            }

            (**scope_iterator)[key]= value;
            #ifdef DEBUGGING
            cout << "new value is " << key << " = " << (**scope_iterator)[key] << endl;
            #endif
        }   
        else if(eat_tokens_if_match({ "_delay", "WHITE SPACE", "NUMBER", "NEW LINE"})){
            
            string time_delay = get_relative_token_content(-2);
            int time_delay_converted = atoi(time_delay.c_str()) * 10000;
            cout<<"o valor de time delay eh  "<<time_delay<<endl;
            usleep(time_delay_converted);
            cout<<"testando delay"<<endl;
        }
        // else if( eat_tokens_if_match( { "FIRST TOKEN TYPE", "_token content" } )){
        //      Do something;
        // }
        else if((*token_iterator)->type == "END"){
            break;
        }

        ++iterations;

    }while( iterations < 200);

}
