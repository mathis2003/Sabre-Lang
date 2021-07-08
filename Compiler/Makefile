CC=gcc

sabre_compiler: compiler.c lexer.c parser.c pretty_printer.c parser_commons.c string_commons.c
	$(CC) compiler.c lexer.c parser.c pretty_printer.c parser_commons.c string_commons.c -o compiler

clean:
	rm compiler
