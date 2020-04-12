%{
	#include <stdio.h>
	void foo(){
		printf("Trabalho 1 - Lex:\n");
	}
%}
digit [0-9]
letter [a-zA-Z]
space [ \t\n]+
sign [+, -]
num_int -?{digit}({digit})*
num_real -?{digit}+"."{digit}+
word [a-zA-Z][a-zA-Z0-9_]*
palavra_reservada ("program")|("var")|("if")|("else")|("then")|("while")|("do")|("for")|("begin")|("end")|("integer")|("real")|("read")|("write")|("ident")|("repeat")|("writeln")|("nomep")|("readln")|("until")
simb (":")|("=")|(";")|(",")|("(")|(")")|(">")|(">=")|("<")|("<=")|("+")|("-")|("/")|("*")|("{")|("}")|("!")|("%")
invalid_simb ("@")|("#")|("$")|("&")
err ({word}*{num_int}*"."{word}+{num_int}+)
%%
{num_int} {printf("%s - id_num_int\n", yytext);}
{num_real} {printf("%s - id_num_real\n", yytext);}
{palavra_reservada} {printf("%s - palavra reservada\n", yytext);}
{simb} {printf("%s - simbolo\n", yytext);}
{word} {printf("%s - id\n", yytext);}
{invalid_simb} {printf("%s - simbolo invalido\n", yytext);}
{err} {printf("%s - error lexico\n", yytext);}
{space}		;
%%
int main()
{
	foo();
	yylex();
	return 0;
}
