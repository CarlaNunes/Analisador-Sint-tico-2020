#include <iostream>
#include <fstream>
#include <string> 
#include <vector> 

//#include "compilador.h"

using namespace std;

int count_tokens = 0;
string name_token = "";

void token(string word);

/* Classe para reconhecer numeros */
class Numbers {
    private:
        bool frag_dot = true;
        int asc_dot = 46;

    public:
        int asc_begin = 48;
        int asc_end = 57;
        
        bool AutomatoNumInt(string number); 
        bool AutomatoNumReal(string number); 
};

/* Classe para reconhecer a pontuacao(simbolos) */
class Punctuation {
    private:
        int asc_plus = 43;
        int asc_less = 45;
        int asc_mult = 42;
        int asc_share = 47;
        int asc_big = 62;
        int asc_small = 60;
        int asc_open = 40;
        int asc_close = 41;
        int asc_dot = 46;
        int asc_tdot = 58;
        int asc_cdot = 59;
        int asc_virg = 44;
        int asc_equal = 61;

        bool frag_pont = true;
    public:
        int asc_begin0 = 58;
        int asc_end0 = 62;

        int asc_begin1 = 40;
        int asc_end1 = 47;

        bool AutomatoSimPlus(string punctuation);
        bool AutomatoSimLess(string punctuation);
        bool AutomatoSimMult(string punctuation);
        bool AutomatoSimShare(string punctuation);
        bool AutomatoSimBig(string punctuation);
        bool AutomatoSimSmall(string punctuation);
        bool AutomatoSimOpen(string  punctuation);
        bool AutomatoSimClose(string punctuation);
        bool AutomatoSimDot(string punctuation);
        bool AutomatoSimTDot(string punctuation);
        bool AutomatoSimCDot(string punctuation);
        bool AutomatoSimVirg(string punctuation);
        bool AutomatoSimEqual(string punctuation); 
        bool AutomatoSimAtrib(string punctuation);
        bool AutomatoSimBigEqual(string punctuation);
        bool AutomatoSimSmallEqual(string punctuation);
        bool AutomatoSimDif(string punctuation);          
};

class Reserved {
    private:
        string program = "program";
        string begin = "begin";
        string end = "end";
        string var = "var";
        string ident_else = "else";
        string read = "read";
        string write = "write";
        string ident_if = "if";
        string then = "then";
        string ident_while = "while";
        string ident_do = "do";
        string ident_for = "for";
        string to = "to";
        string procedure = "procedure";
        string integer = "integer";
        string real = "real";


    public:
        int asc_begin0 = 65;
        int asc_end0 = 90;

        int asc_begin1 = 97;
        int asc_end1 = 122;

        int asc_begin2 = 48;
        int asc_end2 = 57;
        bool AutomatoIdent(string number); 
};


int main(int argc, char *argv[]) 
{ 
    string word, word_aux;
    string::iterator it;
    char c;
    bool flag_2s = false; // flag para lidar com tokens que são símbolos de 2 caracteres (eg. <>, >=)
    bool flag_comment = false;

    ifstream file (argv[1]); // Abre o arquivo

    if (file.is_open()) // Testa se o arquivo foi aberto corretamente
    {
        while (! file.eof() ) // Verifica se nao esta no final do arquivo
        {
            while(file >> word_aux) // Itera sobre o arquivo, palavra por palavra
            {
                //cout << "Original: " << word_aux << endl;

                /* 
                As palavras retornadas podem conter mais de um 1 token, 
                pois a separação é feita com base em espaços em branco e nem 
                sempre os tokens do programa tem espaços em branco entre si
                Exemplo: var x:integer; 
                        - Retornará: "var" e "x:integer;"
                O próximo for itera sobre a palavra retornada, caractere por
                caractere, para conferir se ela não contém mais tokens
                */
                for(it = word_aux.begin(); it != word_aux.end(); ++it)
                {
                    c = *it;

                    // Lida com os simbolos mais comuns
                    if ( (flag_comment == false) && (flag_2s == false) && ( (c == '=') || (c == ',') || (c == ';') || (c == '(') || (c == ')') || (c == '+') || (c == '-') || (c == '/') || (c == '*') ) )
                    {
                        if(word != "") token(word);
                        word = "";
                        word.push_back(c);
                        if( it != (word_aux.end()-1) )
                        {
                            if(word != "") token(word);
                            word = "";
                        }
                    }
                    // Lida com os simbolos que podem começar tokens de 2 caracteres
                    else if ( (flag_comment == false) && (flag_2s == false) && ( (c == ':') || (c == '<') || (c == '>')) )
                    {
                        if(word != "") token(word); 
                        word = "";
                        word.push_back(c);
                        flag_2s = true;
                    }
                    // Lida com o caso de ser identificado um possível simbolo de 2 caracteres
                    else if ( (flag_comment == false) && (flag_2s == true) )
                    {
                        if( ((word == ":" ) && (c == '='))  || ((word == "<" ) && (c == '>')) || ((word == ">" ) && (c == '=')) || ((word == "<" ) && (c == '=')) )
                        {
                            word.push_back(c);
                            if(word != "") token(word);
                            word = "";
                        }
                        else
                        {
                            if(word != "") token(word);
                            word = "";
                            word.push_back(c);
                        }
                        flag_2s = false;
                    }
                    else if ( (flag_comment == false) && (c == '.') && (word=="end") )
                    {
                        if(word != "") token(word);
                        word = "";
                        word.push_back(c);
                    }
                    else if ( (flag_comment == false) && (c == '{') )
                        flag_comment = true;
                    else if ( (flag_comment == true) && (c == '}') )
                        flag_comment = false;
                    // Caracteres normais
                    else if (flag_comment == false) word.push_back(c);
                }

                if(word != "") token(word);
                word = "";
            }
        }
        file.close();
    }

    else cout << "Erro ao ler o programa"; 

    return 0;
} 


void token(string word)
{
    /* Objeto que reconhece numeros */
    Numbers num;
    bool flag_num;

    /* Objeto que reconhece pontuação (simbolos) */
    Punctuation pont;
    bool flag_pont;

    Reserved ident;
    bool flag_ident;


    /* Descobrindo se o numero asc da primeira letra da palavra é um numero
        e caso for, chama o automato que checa se é inteiro ou real*/
    if(num.asc_begin <= int (*word.begin()) &&  num.asc_end >= int (*word.begin() )){
        

        flag_num = num.AutomatoNumInt(word);

        if(flag_num == false)
            flag_num = num.AutomatoNumReal(word);

        if(flag_num == false)
            name_token = "erro(número não permitido)";
    }

    /* Descobrindo se o numero asc da primeira letra da palavra é uma pontuacao
        caso for, são chamados os automatos para reconhecer cada pontuação */
    else if(pont.asc_begin0 <= int (*word.begin()) &&  pont.asc_end0 >= int (*word.begin()) || pont.asc_begin1 <= int (*word.begin()) &&  pont.asc_end1 >= int (*word.begin() )){       
        flag_pont = pont.AutomatoSimPlus(word);

        if(flag_pont == false)
            flag_pont = pont.AutomatoSimLess(word);
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimMult(word);
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimShare(word);
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimBig(word);
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimSmall(word);
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimOpen(word);
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimClose(word);
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimDot(word);
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimTDot(word);
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimCDot(word);
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimVirg(word);
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimEqual(word);
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimAtrib(word);    
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimBigEqual(word);
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimSmallEqual(word);
        if(flag_pont == false)
            flag_pont = pont.AutomatoSimDif(word); 
        if(flag_pont == false)
            name_token = "erro(simbolo não permitido)";
    }   

    else if (ident.asc_begin0 <= int (*word.begin()) &&  ident.asc_end0 >= int (*word.begin()) || ident.asc_begin1 <= int (*word.begin()) &&  ident.asc_end1 >= int (*word.begin() )){   
        flag_ident = ident.AutomatoIdent(word);
        if(flag_ident == false)
            name_token = "erro(identificador não permitido)";
    }

    else 
        name_token = "erro(caractere não permitido)";
    
    cout << "Token " << count_tokens << ": " << word << ", " << name_token << endl;

    count_tokens++;
    name_token = "";
}


/* Declarando os metodos da classe que reconhece numeros */
bool Numbers::AutomatoNumInt(string number){
    string::iterator it_num;

    for(it_num = number.begin(); it_num != number.end(); ++it_num){
        if( asc_begin > *it_num || asc_end < *it_num )
            return false;
    }
    name_token = "num_int";
    return true;
}

bool Numbers::AutomatoNumReal(string number){
    string::iterator it_num;
    frag_dot = true;

    for(it_num = number.begin(); it_num != number.end(); ++it_num){
        if( asc_begin > *it_num || asc_end < *it_num ){
            if( asc_dot == *it_num && frag_dot == true)
                frag_dot = false;
            else 
                return false;
        }      
    }
    name_token = "num_real";
    return true;
}

/* Declarando os metodos da classe que reconhece pontuação */
bool Punctuation::AutomatoSimPlus(string punctuation){
    if(asc_plus == (*punctuation.begin())){
        name_token = "simb_soma";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimLess(string punctuation){
    if(asc_less == (*punctuation.begin())){
        name_token = "simb_sub";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimMult(string punctuation){
    if(asc_mult == (*punctuation.begin())){
        name_token = "simb_mult";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimShare(string punctuation){
    if(asc_share == (*punctuation.begin())){
        name_token = "simb_div";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimBig(string punctuation){
    if(punctuation.length() == 1 && asc_big == (*punctuation.begin())){
        name_token = "simb_maior";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimSmall(string punctuation){
    if(punctuation.length() == 1 && asc_small == (*punctuation.begin())){
        name_token = "simb_menor";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimOpen(string punctuation){
    if(asc_open == (*punctuation.begin())){
        name_token = "simb_apar";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimClose(string punctuation){
    if(asc_close == (*punctuation.begin())){
        name_token = "simb_fpar";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimDot(string punctuation){
    if(asc_dot == (*punctuation.begin())){
        name_token = "simb_pont";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimTDot(string punctuation){
    if(punctuation.length() == 1 && asc_tdot == (*punctuation.begin())){
        name_token = "simb_dp";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimCDot(string punctuation){
    if(asc_cdot == (*punctuation.begin())){
        name_token = "simb_pv";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimVirg(string punctuation){
    if(asc_virg == (*punctuation.begin())){
        name_token = "simb_virg";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimEqual(string punctuation){
    if(asc_equal == (*punctuation.begin())){
        name_token = "simb_igual";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimAtrib(string punctuation){
    if(punctuation.length() == 2 && (*punctuation.begin() == asc_tdot) && (punctuation.at(1) == asc_equal)){
        name_token = "simb_atri";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimBigEqual(string punctuation){
    if(punctuation.length() == 2 && (*punctuation.begin() == asc_big) && (punctuation.at(1) == asc_equal)){
        name_token = "simb_maig";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimSmallEqual(string punctuation){
    if(punctuation.length() == 2 && (*punctuation.begin() == asc_small) && (punctuation.at(1) == asc_equal)){
        name_token = "simb_meig";
        return true;
    }
    return false;
}

bool Punctuation::AutomatoSimDif(string punctuation){
    if(punctuation.length() == 2 && (*punctuation.begin() == asc_small) && (punctuation.at(1) == asc_big)){
        name_token = "simb_dif";
        return true;
    }
    return false;
}

bool Reserved::AutomatoIdent(string ident){
    string::iterator it_ident;
    for(it_ident = ident.begin(); it_ident != ident.end(); ++it_ident){
        if( asc_begin2 > *it_ident || (asc_end2 < *it_ident && asc_begin0 > *it_ident) || asc_end1 < *it_ident || (asc_end0 < *it_ident && asc_begin1 > *it_ident) )
            return false;    
    }
    if (ident.length() == program.length() && program.compare(ident) == 0){
        name_token = "simb_program";
        return true;
    }
    else if(ident.length() == end.length() && end.compare(ident) == 0){
        name_token = "simb_end";
        return true;
    }
    else if(ident.length() == begin.length() && begin.compare(ident) == 0){
        name_token = "simb_begin";
        return true;
    }
    else if(ident.length() == var.length() && var.compare(ident) == 0){
        name_token = "simb_var";
        return true;
    }
    else if(ident.length() == ident_else.length() && ident_else.compare(ident) == 0){
        name_token = "simb_else";
        return true;
    }
    else if(ident.length() == read.length() && read.compare(ident) == 0){
        name_token = "simb_read";
        return true;
    }
    else if(ident.length() == write.length() && write.compare(ident) == 0){
        name_token = "simb_write";
        return true;
    }
    else if(ident.length() == ident_if.length() && ident_if.compare(ident) == 0){
        name_token = "simb_if";
        return true;
    }
    else if(ident.length() == then.length() && then.compare(ident) == 0){
        name_token = "simb_then";
        return true;
    }
    else if(ident.length() == ident_while.length() && ident_while.compare(ident) == 0){
        name_token = "simb_while";
        return true;
    }
    else if(ident.length() == ident_do.length() && ident_do.compare(ident) == 0){
        name_token = "simb_do";
        return true;
    }
    else if(ident.length() == ident_for.length() && ident_for.compare(ident) == 0){
        name_token = "simb_for";
        return true;
    }
    else if(ident.length() == to.length() && to.compare(ident) == 0){
        name_token = "simb_to";
        return true;
    }
    else if(ident.length() == procedure.length() && procedure.compare(ident) == 0){
        name_token = "simb_procedure";
        return true;
    }
    else if(ident.length() == integer.length() && integer.compare(ident) == 0){
        name_token = "simb_integer";
        return true;
    }
    else if(ident.length() == real.length() && real.compare(ident) == 0){
        name_token = "simb_real";
        return true;
    }
    name_token = "id";
    return true;
}
        