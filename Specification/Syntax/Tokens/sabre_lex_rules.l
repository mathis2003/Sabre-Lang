
%{

enum {
	IDENTIFIER = 1,
	NUMBER,

	COMMA,
	DOT,
	SEMI_COLON,
	COLON,
	PLUS,
	MINUS,
	ASTERISK,
	SLASH,
	DOLLAR_SIGN,
	AT_SIGN,
	EQUALS,
	OPEN_PAREN,
	CLOSE_PAREN,
	OPEN_CURLY,
	CLOSE_CURLY,
	OPEN_SQUARE_BRACKET,
	CLOSE_SQUARE_BRACKET,
	OPEN_ANGLE_BRACKET,
	CLOSE_ANGLE_BRACKET	
};


%}



%%


[_a-zA-Z][_a-zA-Z0-9]+?		return IDENTIFIER;
[0-9]+				return NUMBER;

[,]				return COMMA;
[.]				return DOT;
[;]				return SEMI_COLON;
[:]				return COLON;
[+]				return PLUS;
[-]				return MINUS;
[*]				return ASTERISK;
[/]				return SLASH;
[$]				return DOLLAR_SIGN;
[@]				return AT_SIGN;
[=]				return EQUALS;
[(]				return OPEN_PAREN;
[)]				return CLOSE_PAREN;
[{]				return OPEN_CURLY;
[}]				return CLOSE_CURLY;
[[]				return OPEN_SQUARE_BRACKET;
[\]]				return CLOSE_SQUARE_BRACKET;
[<]				return OPEN_ANGLE_BRACKET;
[>]				return CLOSE_ANGLE_BRACKET;

[ \t\n\r]			;

.				printf("unexpected character\n");

%%

int yywrap(void) {
	return 1;
}

