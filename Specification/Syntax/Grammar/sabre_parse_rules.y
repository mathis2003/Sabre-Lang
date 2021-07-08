
%{

#include <stdio.h>
int yylex();

}%

%start program
%type declarations
%type statements

%%

program		: declararions statements
	;

declarations	: declaration
     		| declaration declarations
		;

declaration	: identifier ':' keyword
	   	;

statements	: statement
	   	| statement statements
		;

statement	: expression ';'
	  	;

expression	: literal
	   	| assignment
	   	| binop
		| ternop
		;

literal		: number
	 	| character
		| string
		| function_pointer

assignment	: identifier '=' expression
	  	;

binop		: sum
       		| subtraction
		| multiplication
		| division

sum		: expression '+' expression
     		;

subtraction	: expression '-' expression
	    	;

multiplication	: expression '*' expression
	       	;

division	: expression '/' expression
	 	;

ternop		: "if" expression expression "else" expression ';'


%%

int main() {
	return yyparse();
}

