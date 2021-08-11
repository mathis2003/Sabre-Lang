CC=gcc

sabre_compiler: compiler.c ./Lexer/lexer.c ./Parser/parser.c ./Parser/parser_commons.c ./PrettyPrinter/pretty_printer.c ./CodeGen/code_gen.c ./Commons/string_commons.c
	$(CC) compiler.c ./Lexer/lexer.c ./Parser/parser.c ./Parser/parser_commons.c ./PrettyPrinter/pretty_printer.c ./CodeGen/code_gen.c ./Commons/string_commons.c -o compiler

run: output.c
	./compiler sabre_examples.txt output.c output.h
	gcc output.c -o sabre_app
	./sabre_app
	
	
clean:
	rm compiler
