
// INCLUDES
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "string_commons.h"


// LEXER
char* cur_file_name;
int cur_line_in_file = 1;
TokenArr* lex_code(char* file_name) {
    cur_file_name = file_name;
    TokenArr* result_tok_arr;
    
    
    /*--------------------------------------------------------------------------------------------------------------------------*/
    /* initialize result_tok_arr and write file contents to its StringBucket                                                    */
    /*--------------------------------------------------------------------------------------------------------------------------*/
    {
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
    
    }
    
    /*--------------------------------------------------------------------------------------------------------------------------*/
    /* StringBucket of result_tok_arr should be filled,                                                                         */
    /* start lexing the entire string of code in StringBucket                                                                   */
    /*--------------------------------------------------------------------------------------------------------------------------*/
    char* code_string = result_tok_arr->str_bucket.strings; // using a shorter name for the pointer to the character array in the StringBucket
    for (int i = 0; i < result_tok_arr->str_bucket.bucket_capacity; i++) {
        
        // skip whitespace
        if (is_white_space(code_string[i]))                                              { skip_white_space(code_string, &i);                     continue; }
        
        // skip comment
        else if (code_string[i] == '-' && code_string[i+1] == '-')                       { skip_line_comment(code_string, &i);                    continue; }
        if (code_string[i] == '|' && code_string[i+1] == '"' && code_string[i+2] == '"') { skip_block_comment(code_string, &i);                   continue; }
        
        // lex code
        else if (is_identifier(code_string[i], 1))                                       { add_identifier_token(result_tok_arr, code_string, &i); continue; }
        else if (is_number(code_string[i]))                                              { add_number_token(result_tok_arr, code_string, &i);     continue; }
        else if (code_string[i] == '\"')                                                 { add_string_token(result_tok_arr, code_string, &i);     continue; }
        else if (code_string[i] == '\'')                                                 { add_char_token(result_tok_arr, code_string, &i);       continue; }
        else if (code_string[i] == ',')                                                  { add_comma_token(result_tok_arr);                       continue; }
        else if (code_string[i] == '.')                                                  { add_dot_token(result_tok_arr);                         continue; }
        else if (code_string[i] == ';')                                                  { add_semi_colon_token(result_tok_arr);                  continue; }
        else if (code_string[i] == ':')                                                  { add_colon_token(result_tok_arr);                       continue; }
        else if (code_string[i] == '+')                                                  { add_plus_token(result_tok_arr);                        continue; }
        else if (code_string[i] == '-')                                                  { add_minus_token(result_tok_arr);                       continue; }
        else if (code_string[i] == '*')                                                  { add_asterisk_token(result_tok_arr);                    continue; }
        else if (code_string[i] == '/')                                                  { add_slash_token(result_tok_arr);                       continue; }
        else if (code_string[i] == '$')                                                  { add_dollar_sign_token(result_tok_arr);                 continue; }
        else if (code_string[i] == '@')                                                  { add_at_sign_token(result_tok_arr);                     continue; }
        else if (code_string[i] == '=')                                                  { add_equals_token(result_tok_arr);                      continue; }
        else if (code_string[i] == '(')                                                  { add_open_paren_token(result_tok_arr);                  continue; }
        else if (code_string[i] == ')')                                                  { add_close_paren_token(result_tok_arr);                 continue; }
        else if (code_string[i] == '{')                                                  { add_open_curly_token(result_tok_arr);                  continue; }
        else if (code_string[i] == '}')                                                  { add_close_curly_token(result_tok_arr);                 continue; }
        else if (code_string[i] == '[')                                                  { add_open_square_bracket_token(result_tok_arr);         continue; }
        else if (code_string[i] == ']')                                                  { add_close_square_bracket_token(result_tok_arr);        continue; }
        else if (code_string[i] == '<')                                                  { add_open_angle_bracket_token(result_tok_arr);          continue; }
        else if (code_string[i] == '>')                                                  { add_close_angle_bracket_token(result_tok_arr);         continue; }
        
    }
    
    
    return result_tok_arr;
}


/*-------------------------------------------------------------------------------------------------------------------------------*/
/* white space and comment skipping functions                                                                                    */
/*-------------------------------------------------------------------------------------------------------------------------------*/

void skip_white_space(char* code_str, int* cur_index) {
    while (is_white_space(code_str[(*cur_index)])) {
        if (code_str[(*cur_index)] == '\n') cur_line_in_file++;
        (*cur_index)++;
    }
    
    (*cur_index)--;
}

void skip_line_comment(char* code_str, int* cur_index) {
    (*cur_index)++;
    while (code_str[(*cur_index)] != '\n') (*cur_index)++;
    
    cur_line_in_file++;
}

void skip_block_comment(char* code_str, int* cur_index) {
    int open_comments = 1;
    (*cur_index) += 2;
    
    while (open_comments) {
        if (code_str[(*cur_index)] == '\"' && code_str[(*cur_index)+1] == '\"' && code_str[(*cur_index)+2] == '|') {
            open_comments--;
            (*cur_index) += 2;
        }
         else if( code_str[(*cur_index)] == '|' && code_str[(*cur_index)+1] == '\"' && code_str[(*cur_index)+2] == '\"') {
             open_comments++;
             (*cur_index) += 2;
         }
        else {
            if (code_str[(*cur_index)] == '\n') cur_line_in_file++;
            (*cur_index)++;
        }
    }
}


/*-------------------------------------------------------------------------------------------------------------------------------*/
/* Token adding functions                                                                                                        */
/*-------------------------------------------------------------------------------------------------------------------------------*/

void init_tok(Token* tok, enum TokenType token_type, char* name_str, int name_str_length) {
    tok->file_name = cur_file_name;
    tok->line = cur_line_in_file;
    tok->tok_type = token_type;
    
    tok->name_str.str_start = name_str;
    tok->name_str.str_length = name_str_length;
}

void add_identifier_token(struct TokenArr* tok_arr, char* code_str, int* cur_index) {
    // create token
    Token tok;
    
    // get name length
    int start_index = (*cur_index);
    for ( ; is_identifier(code_str[((*cur_index))], 0); (*cur_index)++) {
        // also make sure the cur_index hasn't reached the maximum of the string bucket, else: break
        if ((*cur_index) >= tok_arr->str_bucket.bucket_capacity) {
            break;
        }
    }
    
    init_tok(&tok, TOK_IDENTIFIER, &(code_str[start_index]), ((*cur_index) - start_index));
    
    (*cur_index)--;
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_number_token(struct TokenArr* tok_arr, char* code_str, int* cur_index) {
    // create token
    Token tok;
    
    // get name length
    int start_index = (*cur_index);
    for ( ; is_number(code_str[((*cur_index))]); (*cur_index)++) {
        // also make sure the cur_index hasn't reached the maximum of the string bucket, else: break
        if ((*cur_index) >= tok_arr->str_bucket.bucket_capacity) {
            break;
        }
    }
    
    init_tok(&tok, TOK_NUMBER, &(code_str[start_index]), ((*cur_index) - start_index));
    
    (*cur_index)--;
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_string_token(struct TokenArr* tok_arr, char* code_str, int* cur_index) {
    (*cur_index)++;
    
    // create token
    Token tok;
    
    // get name length
    int start_index = (*cur_index);
    for ( ; code_str[(*cur_index)] != '\"'; (*cur_index)++) {
        // also make sure the cur_index hasn't reached the maximum of the string bucket, else: break
        if ((*cur_index) >= tok_arr->str_bucket.bucket_capacity) {
            break;
        }
    }
    
    init_tok(&tok, TOK_STRING, &(code_str[start_index]), ((*cur_index) - start_index));
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_char_token(struct TokenArr* tok_arr, char* code_str, int* cur_index) {
    (*cur_index)++;
    
    // create token
    Token tok;
    
    // get name length
    int start_index = (*cur_index);
    for ( ; code_str[(*cur_index)] != '\''; (*cur_index)++) {
        // also make sure the cur_index hasn't reached the maximum of the string bucket, else: break
        if ((*cur_index) >= tok_arr->str_bucket.bucket_capacity) {
            break;
        }
    }
    
    init_tok(&tok, TOK_CHAR, &(code_str[start_index]), ((*cur_index) - start_index));
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_comma_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_COMMA, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_dot_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_DOT, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_semi_colon_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_SEMI_COLON, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_colon_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_COLON, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_plus_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_PLUS, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_minus_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_MINUS, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_asterisk_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_ASTERISK, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_slash_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_SLASH, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_dollar_sign_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_DOLLAR_SIGN, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_at_sign_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_AT_SIGN, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_equals_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_EQUALS, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_open_paren_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_OPEN_PAREN, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_close_paren_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_CLOSE_PAREN, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_open_curly_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_OPEN_CURLY, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_close_curly_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_CLOSE_CURLY, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_open_square_bracket_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_OPEN_SQUARE_BRACKET, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_close_square_bracket_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_CLOSE_SQUARE_BRACKET, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_open_angle_bracket_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_OPEN_ANGLE_BRACKET, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}

void add_close_angle_bracket_token(struct TokenArr* tok_arr) {
    // create token
    Token tok;
    init_tok(&tok, TOK_CLOSE_ANGLE_BRACKET, NULL, 0);
    
    // add token to dynamic token array
    add_tok_to_arr(tok_arr, &tok);
}


/*-------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------*/

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


/*-------------------------------------------------------------------------------------------------------------------------------*/
/* Common needed functions                                                                                                       */
/*-------------------------------------------------------------------------------------------------------------------------------*/

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
