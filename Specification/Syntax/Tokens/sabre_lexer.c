
#include <stdio.h>

extern int yylex();
extern int yylineno;
extern char* yytext;

char *keywords[] = {"i8", "i16", "i32", "fn", "if", "else"};

int main() {
	int token;

	token = yylex();
	while(token) {
		printf("%d\t%s\n", token, yytext);
		token = yylex();
	}

	return 0;
}

