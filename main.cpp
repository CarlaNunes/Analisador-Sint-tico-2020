#include <iostream>
#include <fstream>
#include <string> 
#include <vector> 

using namespace std;

int count_tokens = 0;

void token(string word);

int main(int argc, char *argv[]) 
{ 
    // filestream variable file 
    string line, word, word_aux;
    string::iterator it;
    char c;
    int count_line = 0;
    bool flag = false;

    ifstream file (argv[1]);

    if (file.is_open())
    {
        while (! file.eof() )
        {
            while(file >> word_aux)
            {
                //cout << "Original: " << word_aux << endl;

                for(it = word_aux.begin(); it != word_aux.end(); ++it)
                {
                    c = *it;
                    if ( (flag == false) && (c == '=') || (c == ';') || (c == '.') || (c == '(') || (c == ')') || (c == '+') || (c == '-') || (c == '/') || (c == '*') || (c == '>') )
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
                    else if ( (flag == false) && (c == ':') || (c == '<') )
                    {
                        if(word != "") token(word);
                        word = "";
                        word.push_back(c);
                        flag = true;
                    }
                    else if (flag == true)
                    {
                        if( ((word == ":" ) && (c == '='))  || ((word == "<" ) && (c == '>')) )
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