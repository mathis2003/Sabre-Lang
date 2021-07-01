
// INCLUDES
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"


// LEXER
char* cur_file_name;
int cur_line_in_file = 1;
TokenArr* lex_code(char* file_name) {
    cur_file_name = file_name;
    
    TokenArr* result_tok_arr;
    int amount_of_chars_in_file = 0;
    
    
    /*----------------------------------------------------------------------------------------------------------------------*/
    /* Loop through the entire file, to see how many characters it contains                                                 */
    /*----------------------------------------------------------------------------------------------------------------------*/
    
    {
        FILE* file_ptr = fopen(file_name, "r");
        if (file_ptr == NULL) { return NULL; }
    
        {
            int ch;
            while ((ch = fgetc(file_ptr)) != EOF) amount_of_chars_in_file++;
        }
        
        fclose(file_ptr);
    }
    
    
    /*----------------------------------------------------------------------------------------------------------------------*/
    /* Now we have the amount of characters in the file,                                                                    */
    /* initialize result_tok_arr and give the characters of the file to its StringBucket                                    */
    /*----------------------------------------------------------------------------------------------------------------------*/
    
    result_tok_arr = malloc(sizeof(TokenArr));
    init_tok_arr(result_tok_arr, amount_of_chars_in_file);
    {
        FILE* file_ptr = fopen(file_name, "r");
        if (file_ptr == NULL) { return NULL; }
    
        {
            int ch;
            while ((ch = fgetc(file_ptr)) != EOF) {
                // fill StringBucket of result_tok_arr
                if (1 == add_char_to_str_bucket(&(result_tok_arr->str_bucket), ch)) { return NULL; } // StringBucket was already full
            }
        }
    
        fclose(file_ptr);
    }
    
    /*----------------------------------------------------------------------------------------------------------------------*/
    /* StringBucket of result_tok_arr should be filled,                                                                     */
    /* start lexing the entire string of code in StringBucket                                                               */
    /*----------------------------------------------------------------------------------------------------------------------*/

    char* code_string = result_tok_arr->str_bucket.strings; // using a shorter name for the pointer to the character array in the StringBucket
    for (int i = 0; i <= result_tok_arr->str_bucket.bucket_capacity; i++) {
        
        // skip whitespace
        if (is_white_space(code_string[i]))   { skip_white_space(code_string, &i);                     continue; }
        
        // skip comment
        
        // lex code
        if (is_identifier(code_string[i], 1)) { add_identifier_token(result_tok_arr, code_string, &i); continue; }
        else if (is_number(code_string[i]))   { add_number_token(result_tok_arr, code_string, &i);     continue; }
        
    }
    
    
    
    
    return result_tok_arr;
}


/*--------------------------------------------------------------------------------------------------------------------------*/
/* white space and comment skipping functions                                                                               */
/*--------------------------------------------------------------------------------------------------------------------------*/

void skip_white_space(char* code_str, int* cur_index) {
    while (is_white_space(code_str[(*cur_index)])) {
        if (code_str[(*cur_index)] == '\n') cur_line_in_file++;
        (*cur_index) += 1;
    }
    
    (*cur_index)--;
}


/*--------------------------------------------------------------------------------------------------------------------------*/
/* Token adding functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------------*/

void add_identifier_token(struct TokenArr* tok_arr, char* code_str, int* cur_index) {
    
    // create token
    Token tok;
    tok.file_name = cur_file_name;
    tok.line = cur_line_in_file;
    tok.tok_type = TOK_IDENTIFIER;
    tok.name = &(code_str[(*cur_index)]);
    
    int start_index = (*cur_index);
    for ( ; is_identifier(code_str[((*cur_index))], 0); (*cur_index)++)
        ;
    
    tok.name_length = (*cur_index) - start_index;
    
    (*cur_index)--;
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
    
}

void add_number_token(struct TokenArr* tok_arr, char* code_str, int* cur_index) {
    
    // create token
    Token tok;
    tok.file_name = cur_file_name;
    tok.line = cur_line_in_file;
    tok.tok_type = TOK_NUMBER;
    tok.name = &(code_str[(*cur_index)]);
    
    int start_index = (*cur_index);
    for ( ; is_number(code_str[((*cur_index))]); (*cur_index)++)
        ;
    
    tok.name_length = (*cur_index) - start_index;
    
    (*cur_index)--;
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
    
}

/*--------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/

int is_identifier(char ch, char is_first) {
    if (is_first) return (ch == '_') || ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
    return (ch == '_') || ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ('0' <= ch && ch <= '9');
}

int is_number(char ch) {
    return ('0' <= ch && ch <= '9');
}

int is_white_space(char ch) {
    return (ch == ' ') || (ch == '\t') || (ch == '\n') || (ch == '\r');
}


/*--------------------------------------------------------------------------------------------------------------------------*/
/* Common needed functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------------*/

void init_str_bucket(struct StringBucket* str_bucket, int str_bucket_capacity) {
    str_bucket->bucket_capacity = str_bucket_capacity;
    str_bucket->bucket_top = 0;
    str_bucket->strings = malloc(str_bucket->bucket_capacity * sizeof(char));
}

int add_char_to_str_bucket(struct StringBucket* str_bucket, char ch) {
    if (str_bucket->bucket_top >= str_bucket->bucket_capacity) {
        return 1; // ERROR: stringbucket is already full
    }
    
    str_bucket->strings[str_bucket->bucket_top] = ch;
    str_bucket->bucket_top += 1;
    return 0;
}

void init_tok_arr(struct TokenArr* tok_arr, int chars_file_size) {
    tok_arr->size = 0;
    tok_arr->capacity = 50;
    tok_arr->tokens = malloc(tok_arr->capacity * sizeof(Token));
    init_str_bucket(&(tok_arr->str_bucket), chars_file_size);
}

void add_tok_to_arr(struct TokenArr* tok_arr, struct Token* tok) {
    if (tok_arr->size >= tok_arr->capacity) {
        tok_arr->capacity *= 2;
        tok_arr->tokens = realloc(tok_arr->tokens, tok_arr->capacity * sizeof(Token));
    }

    tok_arr->tokens[tok_arr->size] = *tok;
    tok_arr->size += 1;
}

void free_tokens(struct TokenArr* tok_arr) {
    free(tok_arr->tokens);
    free(tok_arr->str_bucket.strings);
    free(tok_arr);
}
