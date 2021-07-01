#ifndef LEXER_H
#define LEXER_H

//#include "lexer_commons.h"



typedef enum TokenType {
	TOK_IDENTIFIER, TOK_NUMBER
} TokenType;


/* Structs */

typedef struct StringBucket {
    int bucket_capacity, bucket_top;
    char* strings;
} StringBucket;

typedef struct Token {
	char* file_name;
	int line;
	enum TokenType tok_type;
	char* name;
} Token;

typedef struct TokenArr {
	int size, capacity;
	struct Token* tokens;

	StringBucket str_bucket;
} TokenArr;



/* Functions */

TokenArr* lex_code(char* file_name);

void init_str_bucket(struct StringBucket* str_bucket, int bucket_capacity);
int add_char_to_str_bucket(struct StringBucket* str_bucket, char ch);

void init_tok_arr(struct TokenArr* tok_arr, int chars_file_size);
void add_tok_to_arr(struct TokenArr* tok_arr, struct Token* tok);


#endif
