CC=gcc

sabre_compiler: compiler.c lexer.c parser.c parser_commons.c pretty_printer.c code_gen.c string_commons.c
	$(CC) compiler.c lexer.c parser.c parser_commons.c pretty_printer.c code_gen.c string_commons.c -o compiler

run: output.c
	./compiler sabre_examples.txt output.c output.h
	
	
clean:
	rm compiler
