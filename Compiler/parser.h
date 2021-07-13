#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "string_commons.h"

typedef enum StatementType {
    STMT_ASSIGN
} StatementType;

typedef enum DataType {
    UINT_8, UINT_16, UINT_32, STRING
} DataType;

/*----------------------------------------------------------------------------------------*/

/* Structs */

typedef struct Statement {
    enum StatementType stmt_type;
} Statement;

typedef struct Scope {
    struct Scope* parent_scope;
    
    int amount_of_nested_scopes;
    struct Scope* scopes;
    
    int amount_of_declarations;
    struct Declaration* declarations;
} Scope;

typedef struct Declaration {
    struct StringStruct identifier;
    enum DataType type;
    
    char is_initialized;
    union {
        int init_int_val;
        char init_char_val;
        //...
    };
} Declaration;

typedef struct EntryPoint {
    struct Scope* scope;
} EntryPoint;

typedef struct Allocators {
    struct MainScopeBucket* scope_bucket;
    struct MainDeclarationBucket* declaration_bucket;
    // also add buckets for data declarations and statements here
} Allocators;

typedef struct Program {
    struct EntryPoint* entry_point;
    struct Scope* global_scope;
    
    struct Allocators allocators;
} Program;


/*----------------------------------------------------------------------------------------*/

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

// this should be visible to the programmer
typedef struct MainDeclarationBucket {
    struct DeclarationBucket* root_bucket;
} MainDeclarationBucket;

// this should be hidden from the programmer
typedef struct DeclarationBucket {
    int size, capacity;
    struct Declaration* declarations;
    struct DeclarationBucket* next_bucket;
    struct DeclarationBucket* prev_bucket;
} DeclarationBucket;

/*----------------------------------------------------------------------------------------*/

/* FUNCTIONS */

struct Program* parse_tokens(struct TokenArr* tok_arr);

void parse_program_node(struct Program* program_node);
struct Scope* parse_scope();
struct Declaration* parse_declaration();

struct Token* next_token();
struct Token* peek_token();
void eat_token(enum TokenType expected_type);
struct Token* cur_token();

void free_AST(struct Program* ast_root);

void init_ast_allocators(Program* program_ptr);

/*----------------------------------------------------------------------------------------*/

struct Scope* add_scope_to_bucket(struct Scope* scope_to_add, struct MainScopeBucket* scope_bucket);
void init_main_scope_bucket(struct Program* program);
void free_scope_bucket(MainScopeBucket* scope_bucket);

struct Declaration* add_declaration_to_bucket(struct Declaration* declaration_to_add, struct MainDeclarationBucket* declaration_bucket);
void init_main_declaration_bucket(struct Program* program);
void free_declaration_bucket(MainDeclarationBucket* declaration_bucket);



#endif
