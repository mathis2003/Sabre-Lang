#ifndef LEXER_H
#define LEXER_H


typedef enum TokenType {
	TOK_IDENTIFIER, TOK_NUMBER, TOK_STRING, TOK_CHAR, TOK_COMMA, TOK_DOT, TOK_SEMI_COLON, TOK_COLON,
    TOK_MINUS, TOK_PLUS, TOK_ASTERISK, TOK_SLASH, TOK_DOLLAR_SIGN, TOK_AT_SIGN, TOK_ASSIGN,
    // TOK_PLUS_EQUALS, TOK_MINUS_EQUALS, ... (extra, not crucial)
    TOK_EQUAL, TOK_NOT_EQUAL, TOK_BIGGER_THAN, TOK_SMALLER_THAN,
    TOK_OPEN_PAREN, TOK_CLOSE_PAREN, TOK_OPEN_CURLY, TOK_CLOSE_CURLY, TOK_OPEN_SQUARE_BRACKET,
    TOK_CLOSE_SQUARE_BRACKET, TOK_OPEN_ANGLE_BRACKET, TOK_CLOSE_ANGLE_BRACKET
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
    int name_length;
} Token;

typedef struct TokenArr {
	int size, capacity;
	struct Token* tokens;

	StringBucket str_bucket;
} TokenArr;



/* Functions */

TokenArr* lex_code(char* file_name);

void skip_white_space(char* code_str, int* cur_index);
void skip_line_comment(char* code_str, int* cur_index);
void skip_block_comment(char* code_str, int* cur_index);

void add_identifier_token(struct TokenArr* tok_arr, char* code_str, int* cur_index);
void add_number_token(struct TokenArr* tok_arr, char* code_str, int* cur_index);

int is_identifier(char ch, char is_first);
int is_number(char ch);
int is_white_space(char ch);

void init_str_bucket(struct StringBucket* str_bucket, int bucket_capacity);
int add_char_to_str_bucket(struct StringBucket* str_bucket, char ch);

void init_tok_arr(struct TokenArr* tok_arr, int chars_file_size);
void add_tok_to_arr(struct TokenArr* tok_arr, struct Token* tok);
void free_tokens(struct TokenArr* tok_arr);

#endif
