
// INCLUDES
#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "string_commons.h"

void print_tokens(TokenArr* tok_arr) {
    
    // Print header
    
    printf("\n\n----------------\n|    TOKENS   |\n----------------\n\n");
    
    // Print body
    
    for (int i = 0; i < tok_arr->size; i++) {
        printf("TOKEN:\n");
        printf("\t-file: %s\n", tok_arr->tokens[i].file_name);
        printf("\t-line: %d\n", tok_arr->tokens[i].line);
        printf("\t-type: ");
        switch (tok_arr->tokens[i].tok_type) {
            case TOK_IDENTIFIER: {
                printf("TOK_IDENTIFIER\n");
                printf("\t-name: ");
                for (int j = 0; j < tok_arr->tokens[i].name_str.str_length; j++) {
                    printf("%c", tok_arr->tokens[i].name_str.str_start[j]);
                }
                printf("\n");
                break;
            }
            
            case TOK_NUMBER: {
                printf("TOK_NUMBER\n");
                printf("\t-name: ");
                for (int j = 0; j < tok_arr->tokens[i].name_str.str_length; j++) {
                    printf("%c", tok_arr->tokens[i].name_str.str_start[j]);
                }
                printf("\n");
                break;
            }
            
            case TOK_STRING: {
                printf("TOK_STRING\n");
                printf("\t-name: ");
                for (int j = 0; j < tok_arr->tokens[i].name_str.str_length; j++) {
                    printf("%c", tok_arr->tokens[i].name_str.str_start[j]);
                }
                printf("\n");
                break;
            }
            
            case TOK_CHAR: {
                printf("TOK_CHAR\n");
                printf("\t-name: ");
                for (int j = 0; j < tok_arr->tokens[i].name_str.str_length; j++) {
                    printf("%c", tok_arr->tokens[i].name_str.str_start[j]);
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

#define PRINT_NODE(node_name, tree_level) \
for (int i = 0; i < tree_level; i++) printf("    "); \
printf("%s", node_name);

void print_fn_literal(struct FnLiteral* fn_literal, int tree_level);

void print_declarations(struct VoidPtrArr* decl_ptr_arr, int tree_level) {
    for (int i = 0; i < decl_ptr_arr->size; i++) {
        PRINT_NODE("-DECLARATION\n", tree_level);
        PRINT_NODE("Name: ", tree_level + 1);
        print_str_struct(&((struct Declaration*)(decl_ptr_arr->void_ptrs[i]))->identifier);
        printf("\n");
        switch (((struct Declaration*)(decl_ptr_arr->void_ptrs[i]))->type) {
            case UINT_8: {
                PRINT_NODE("Type: u8\n", tree_level + 1);
                if (((struct Declaration*)(decl_ptr_arr->void_ptrs[i]))->is_initialized) {
                    PRINT_NODE("Init value: ", tree_level + 1);
                    printf("%ld", ((struct Declaration*)(decl_ptr_arr->void_ptrs[i]))->init_int_val);
                    printf("\n");
                }
                break;
            }
            case UINT_16: {
                PRINT_NODE("Type: u16\n", tree_level + 1);
                if (((struct Declaration*)(decl_ptr_arr->void_ptrs[i]))->is_initialized) {
                    PRINT_NODE("Init value: ", tree_level + 1);
                    printf("%ld", ((struct Declaration*)(decl_ptr_arr->void_ptrs[i]))->init_int_val);
                    printf("\n");
                }
                break;
            }
            case UINT_32: {
                PRINT_NODE("Type: u32\n", tree_level + 1);
                if (((struct Declaration*)(decl_ptr_arr->void_ptrs[i]))->is_initialized) {
                    PRINT_NODE("Init value: ", tree_level + 1);
                    printf("%ld", ((struct Declaration*)(decl_ptr_arr->void_ptrs[i]))->init_int_val);
                    printf("\n");
                }
                break;
            }
            case STRING: {
                PRINT_NODE("Type: String\n", tree_level + 1);
                if (((struct Declaration*)(decl_ptr_arr->void_ptrs[i]))->is_initialized) {
                    PRINT_NODE("Init value: \"", tree_level + 1);
                    print_str_struct(&((struct Declaration*)(decl_ptr_arr->void_ptrs[i]))->init_string);
                    printf("\"\n");
                }
                break;
            }
            case FN_PTR: {
                PRINT_NODE("Type: Fn\n", tree_level + 1);
                if (((struct Declaration*)(decl_ptr_arr->void_ptrs[i]))->is_initialized) {
                    PRINT_NODE("Init value: \n", tree_level + 1);
                    print_fn_literal(((struct Declaration*)(decl_ptr_arr->void_ptrs[i]))->init_fn_ptr, tree_level + 1);
                }
                break;
            }
                
            default:
                break;
        }
    }
}

void print_fn_literal(struct FnLiteral* fn_literal, int tree_level) {
    
    PRINT_NODE("Parameter(s): \n", tree_level + 1);
    for (int i = 0; i < fn_literal->param_decl_ptr_arr.size; i++) {
        PRINT_NODE("Name: ", tree_level + 2);
        print_str_struct(&((struct Declaration*)(fn_literal->param_decl_ptr_arr.void_ptrs[i]))->identifier);
        printf("\n");
        PRINT_NODE("Type: ", tree_level + 2);
        switch (((struct Declaration*)(fn_literal->param_decl_ptr_arr.void_ptrs[i]))->type) {
            case UINT_8: {
                printf("u8\n");
                break;
            }
            case UINT_16: {
                printf("u16\n");
                break;
            }
            case UINT_32: {
                printf("u32\n");
                break;
            }
            case STRING: {
                printf("String\n");
                break;
            }
            case FN_PTR: {
                printf("fn\n");
                break;
            }
                
            default:
                break;
        }
    }
    
    PRINT_NODE("Return Type: ", tree_level + 1);
    switch (fn_literal->return_type) {
        case UINT_8: {
            printf("u8\n");
            break;
        }
        case UINT_16: {
            printf("u16\n");
            break;
        }
        case UINT_32: {
            printf("u32\n");
            break;
        }
        case STRING: {
            printf("String\n");
            break;
        }
        case FN_PTR: {
            printf("fn\n");
            break;
        }
        case UNIT: {
            printf("Unit\n");
            break;
        }
            
        default:
            break;
    }

    
    print_declarations(&(fn_literal->decl_ptr_arr), tree_level+1);
}

void print_entry_point(struct EntryPoint* entry_point) {
    print_declarations(&(entry_point->decl_ptr_arr), 2);
}

void print_parse_tree(Program* program_node) {
    
    // Print header
    printf("\n\n---------------\n|     AST     |\n---------------\n\n");
    
    // Print body
    
    printf("-PROGRAM:\n");
    /// print entry point
    if (program_node->entry_point == NULL) printf("NO ENTRY POINT -> INVALID PROGRAM\n");
    else {
        PRINT_NODE("-ENTRY_POINT:\n", 1);
        print_entry_point(program_node->entry_point);
    }
    /// print global declarations
}
