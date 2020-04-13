#include <iostream>
#include <fstream>
#include <string> 
#include <vector> 

using namespace std;

int count_tokens = 0;

void token(string word);

int main(int argc, char *argv[]) 
{ 
    string word, word_aux;
    string::iterator it;
    char c;
    bool flag = false; // flag para lidar com tokens que são símbolos de 2 caracteres (eg. <>, >=)

    ifstream file (argv[1]); // Abre o arquivo

    if (file.is_open()) // Testa se o arquivo foi aberto corretamente
    {
        while (! file.eof() ) 
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
                    if ( (flag == false) && ( (c == '=') || (c == ';') || (c == '.') || (c == '(') || (c == ')') || (c == '+') || (c == '-') || (c == '/') || (c == '*') ) )
                    {
                        if(word != "") token(word);
                        word = "";
                        word.push_back(c);
                        if( it != (word_aux.end()-1) )
                        {
                            token(word);
                            word = "";
                        }
                    }
                    // Lida com os simbolos que podem começar tokens de 2 caracteres
                    else if ( (flag == false) && ( (c == ':') || (c == '<') || (c == '>')) )
                    {
                        if(word != "") token(word); 
                        word = "";
                        word.push_back(c);
                        flag = true;
                    }
                    // Lida com o caso de ser identificado um possível simbolo de 2 caracteres
                    else if (flag == true)
                    {
                        if( ((word == ":" ) && (c == '='))  || ((word == "<" ) && (c == '>')) || ((word == ">" ) && (c == '=')) || ((word == "<" ) && (c == '=')) )
                        {
                            word.push_back(c);
                            token(word);
                            word = "";
                        }
                        else
                        {
                            token(word);
                            word = "";
                            word.push_back(c);
                        }
                        flag = false;
                    }
                    // Caracteres normais
                    else word.push_back(c);
                }

                token(word);
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
    string::iterator it_2;

    for(it_2 = word.begin(); it_2 != word.end(); ++it_2)
    {

    }

    cout << "Token " << count_tokens << ": " << word << endl;
    count_tokens++;
}