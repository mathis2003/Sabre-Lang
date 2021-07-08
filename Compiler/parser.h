#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum StatementType {
    STMT_ASSIGN
} StatementType;

typedef enum DataType {
    UINT_8, UINT_16, UINT_32
} DataType;


/* Structs */

typedef struct Declaration {
    enum DataType type;
    union {
        char char_val;
        int  int_val;
    };
} Declaration;

typedef struct Statement {
    enum StatementType stmt_type;
} Statement;

typedef struct Scope {
    
    struct Scope* parent_scope;
    
    int amount_of_nested_scopes;
    struct Scope* scopes;
    
    int amount_of_variables;
    char** variables;
    
    
    
} Scope;

typedef struct EntryPoint {
    struct Scope* scope;
} EntryPoint;

typedef struct Allocators {
    struct MainScopeBucket* scope_bucket;
    // also add buckets for data declarations and statements here
} Allocators;

typedef struct Program {
    struct EntryPoint* entry_point;
    struct Scope* global_scope;
    
    struct Allocators allocators;
} Program;



/* Functions */

struct Program* parse_tokens(struct TokenArr* tok_arr);

void parse_program_node(struct Program* program_node);
struct Scope* parse_scope();

struct Token* next_token();
struct Token* peek_token();
void eat_token(enum TokenType expected_type);
struct Token* cur_token();

void free_AST(struct Program* ast_root);

void init_ast_allocators(Program* program_ptr);

/*---------------------------*/


// this should be visible to the programmer
typedef struct MainScopeBucket {
    struct ScopeBucket* root_bucket;
} MainScopeBucket;

// this should be hidden from the programmer
typedef struct ScopeBucket {
    int size, capacity;
    struct Scope* scopes;
    struct ScopeBucket* next_bucket;
    struct ScopeBucket* prev_bucket;
} ScopeBucket;



/* FUNCTIONS */

struct Scope* add_scope_to_bucket(struct Scope* scope_to_add, struct MainScopeBucket* scope_bucket);
void init_main_scope_bucket(struct Program* program);
void free_scope_bucket(MainScopeBucket* scope_bucket);



#endif
