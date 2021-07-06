
%%


[_a-zA-Z][_a-zA-Z0-9]+?		printf("IDENTIFIER\n");
[0-9]+				printf("NUMBER\n");

[,]				printf("COMMA\n");
[.]				printf("DOT\n");
[;]				printf("SEMI_COLON\n");
[:]				printf("COLON\n");
[+]				printf("PLUS\n");
[-]				printf("MINUS\n");
[*]				printf("ASTERISK\n");
[/]				printf("SLASH\n");
[$]				printf("DOLLAR_SIGN\n");
[@]				printf("AT_SIGN\n");
[=]				printf("EQUALS\n");
[(]				printf("OPEN_PAREN\n");
[)]				printf("CLOSE_PAREN\n");
[{]				printf("OPEN_CURLY\n");
[}]				printf("CLOSE_CURLY\n");
[[]				printf("OPEN_SQUARE_BRACKET\n");
[\]]				printf("CLOSE_SQUARE_BRACKET\n");
[<]				printf("OPEN_ANGLE_BRACKET\n");
[>]				printf("CLOSE_ANGLE_BRACKET\n");



%%


