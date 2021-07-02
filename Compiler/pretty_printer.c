
// INCLUDES
#include <stdio.h>

#include "lexer.h"

void print_tokens(TokenArr* tok_arr) {
    for (int i = 0; i < tok_arr->size; i++) {
        printf("TOKEN:\n");
        printf("\t-file: %s\n", tok_arr->tokens[i].file_name);
        printf("\t-line: %d\n", tok_arr->tokens[i].line);
        printf("\t-type: ");
        switch (tok_arr->tokens[i].tok_type) {
            case TOK_IDENTIFIER: {
                printf("TOK_IDENTIFIER\n");
                printf("\t-name: ");
                for (int j = 0; j < tok_arr->tokens[i].name_length; j++) {
                    printf("%c", tok_arr->tokens[i].name[j]);
                }
                printf("\n");
                break;
            }
            
            case TOK_NUMBER: {
                printf("TOK_NUMBER\n");
                printf("\t-name: ");
                for (int j = 0; j < tok_arr->tokens[i].name_length; j++) {
                    printf("%c", tok_arr->tokens[i].name[j]);
                }
                printf("\n");
                break;
            }
            
            case TOK_STRING: {
                printf("TOK_STRING\n");
                printf("\t-name: ");
                for (int j = 0; j < tok_arr->tokens[i].name_length; j++) {
                    printf("%c", tok_arr->tokens[i].name[j]);
                }
                printf("\n");
                break;
            }
            
            case TOK_CHAR: {
                printf("TOK_CHAR\n");
                printf("\t-name: ");
                for (int j = 0; j < tok_arr->tokens[i].name_length; j++) {
                    printf("%c", tok_arr->tokens[i].name[j]);
                }
                printf("\n");
                break;
            }
            
            case TOK_COMMA: {
                printf("TOK_COMMA\n");
                break;
            }
            
            case TOK_DOT: {
                printf("TOK_DOT\n");
                break;
            }
            
            case TOK_SEMI_COLON: {
                printf("TOK_SEMI_COLON\n");
                break;
            }
                
            case TOK_COLON: {
                printf("TOK_COLON\n");
                break;
            }
            
            case TOK_PLUS: {
                printf("TOK_PLUS\n");
                break;
            }
            
            case TOK_MINUS: {
                printf("TOK_MINUS\n");
                break;
            }
            
            case TOK_ASTERISK: {
                printf("TOK_ASTERISK\n");
                break;
            }
            
            case TOK_SLASH: {
                printf("TOK_SLASH\n");
                break;
            }
            
            case TOK_DOLLAR_SIGN: {
                printf("TOK_DOLLAR_SIGN\n");
                break;
            }
            
            case TOK_AT_SIGN: {
                printf("TOK_AT_SIGN\n");
                break;
            }
            
            case TOK_EQUALS: {
                printf("TOK_EQUALS\n");
                break;
            }
            
            case TOK_OPEN_PAREN: {
                printf("TOK_OPEN_PAREN\n");
                break;
            }
            
            case TOK_CLOSE_PAREN: {
                printf("TOK_CLOSE_PAREN\n");
                break;
            }
            
            case TOK_OPEN_CURLY: {
                printf("TOK_OPEN_CURLY\n");
                break;
            }
            
            case TOK_CLOSE_CURLY: {
                printf("TOK_CLOSE_CURLY\n");
                break;
            }
            
            case TOK_OPEN_SQUARE_BRACKET: {
                printf("TOK_OPEN_SQUARE_BRACKET\n");
                break;
            }
            
            case TOK_CLOSE_SQUARE_BRACKET: {
                printf("TOK_CLOSE_SQUARE_BRACKET\n");
                break;
            }
            
            case TOK_OPEN_ANGLE_BRACKET: {
                printf("TOK_OPEN_ANGLE_BRACKET\n");
                break;
            }
            
            case TOK_CLOSE_ANGLE_BRACKET: {
                printf("TOK_CLOSE_ANGLE_BRACKET\n");
                break;
            }
                
            default:
                break;
        }
        
    }
}
