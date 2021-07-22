#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "string_commons.h"


typedef enum StatementType {
    STMT_ASSIGN
} StatementType;

typedef enum DataType {
    UNIT, UINT_8, UINT_16, UINT_32, STRING, FN_PTR
} DataType;

/*----------------------------------------------------------------------------------------*/

/* Structs */

typedef struct VoidPtrArr {
    int size, capacity;
    void** void_ptrs;
    
} VoidPtrArr;

typedef struct FnLiteral {
    struct FnLiteral* parent_scope;
    
    struct VoidPtrArr decl_ptr_arr;
    struct VoidPtrArr stmt_ptr_arr;
    
} FunctionLiteral;

typedef struct Statement {
    enum StatementType stmt_type;
} Statement;

typedef struct Declaration {
    struct StringStruct identifier;
    enum DataType type;
    
    char is_initialized;
    union {
        long int init_int_val;
        char init_char_val;
        struct StringStruct init_string;
        struct FnLiteral init_fn_ptr;
        //...
    };
    
} Declaration;


typedef struct EntryPoint {
    //struct Scope* scope;
    
    struct VoidPtrArr decl_ptr_arr;
    struct VoidPtrArr stmt_ptr_arr;
} EntryPoint;

typedef struct Allocators {
    struct MainFnLiteralBucket* fn_literal_bucket;
    struct MainDeclarationBucket* declaration_bucket;
    // also add buckets for data declarations and statements here
} Allocators;

typedef struct Program {
    struct EntryPoint* entry_point;
    struct VoidPtrArr decl_ptr_arr;
    
    struct Allocators allocators;
} Program;


/*----------------------------------------------------------------------------------------*/

// this should be visible to the programmer
typedef struct MainFnLiteralBucket {
    struct FnLiteralBucket* root_bucket;
} MainFnLiteralBucket;

// this should be hidden from the programmer
typedef struct FnLiteralBucket {
    int size, capacity;
    struct FnLiteral* fn_literals;
    struct FnLiteralBucket* next_bucket;
    struct FnLiteralBucket* prev_bucket;
} FnLiteralBucket;

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
struct EntryPoint* parse_entry_point();
struct FnLiteral* parse_fn_literal(struct FnLiteral* surrounding_scope);
struct Declaration* parse_declaration(struct FnLiteral* surrounding_scope);
struct Statement* parse_statement (struct FnLiteral* surrounding_scope);

struct Token* next_token();
struct Token* peek_token();
void eat_token(enum TokenType expected_type);
struct Token* cur_token();

void free_AST(struct Program* ast_root);

void init_ast_allocators(Program* program_ptr);

/*----------------------------------------------------------------------------------------*/

struct FnLiteral* add_fn_literal_to_bucket(struct FnLiteral* fn_to_add, struct MainFnLiteralBucket* fn_bucket);
void init_main_fn_literal_bucket(struct Program* program);
void free_fn_literal_bucket(MainFnLiteralBucket* fn_literal_bucket);

struct Declaration* add_declaration_to_bucket(struct Declaration* declaration_to_add, struct MainDeclarationBucket* declaration_bucket);
void init_main_declaration_bucket(struct Program* program);
void free_declaration_bucket(MainDeclarationBucket* declaration_bucket);

void init_void_ptr_arr(struct VoidPtrArr* void_ptr_arr);
void add_void_ptr_to_arr(struct VoidPtrArr* void_ptr_arr, void* ptr_to_add);
void free_void_ptr_arr(struct VoidPtrArr* void_ptr_arr);

#endif
