#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "string_commons.h"


typedef enum ExpressionType {
    EXPR_ADD, EXPR_SUB, EXPR_MULT, EXPR_DIV, EXPR_IF_THEN_ELSE, EXPR_FN_CALL,
    EXPR_LOGIC_AND, EXPR_LOGIC_OR,
    EXPR_COND_EQUALS, EXPR_COND_GREATER_EQUALS, EXPR_COND_LOWER_EQUALS, EXPR_COND_GREATER, EXPR_COND_LOWER,
    EXPR_IDENT_LITERAL, EXPR_NUM_LITERAL, EXPR_FN_LITERAL, EXPR_STR_LITERAL,
    EXPR_ASSIGN,
    EXPR_VALUE_OF,
    EXPR_MEMBER_ACCESS
    
} ExpressionType;

typedef enum StatementType {
    STMT_EXPR, STMT_CFI
} StatementType;

typedef enum DataTypeEnum {
    UNIT, UINT_8, UINT_16, UINT_32, STRING, FN_PTR, DATA_BOOL
} DataTypeEnum;

/*----------------------------------------------------------------------------------------*/

/* Structs */
typedef struct DataType {
    // stuff
    unsigned int is_fn_ptr : 1;
    unsigned int is_value : 1;
    struct StringStruct type_name;
    struct FunctionType* fn_type;
} DataType;

typedef struct VoidPtrArr {
    int size, capacity;
    void** void_ptrs;
    
} VoidPtrArr;

typedef struct ImportList {
    // Structure Of Arrays : imported files and their names are parallel in arrays
    int amount_of_imported_files;
    
    struct StringStruct* imported_files;
    char*                is_local_file_flags;
    struct StringStruct* namespaces;
    
} ImportList;

typedef struct ScopeObject {
    char index;
    
    // accessed element
    unsigned int start_lbl : 1;
    unsigned int end_lbl : 1;
    unsigned int exit_lbl : 1;
    unsigned int return_val : 1;
} ScopeObject;

typedef struct ControlFlowIndicator {
    struct ScopeObject scope_obj;
} ControlFlowIndicator;

typedef struct FnLiteral {
    struct FnLiteral* parent_scope;
    
    struct VoidPtrArr param_decl_ptr_arr;
    struct VoidPtrArr decl_ptr_arr;
    struct VoidPtrArr stmt_ptr_arr;
    struct VoidPtrArr struct_type_ptr_arr;
    
    struct ImportList imports;
    
    struct Declaration* return_variable;
    
} FunctionLiteral;

typedef struct FnCall {
    struct Expression* fn_ptr_expr;
    
    struct VoidPtrArr arg_ptr_arr;
} FnCall;

typedef struct TernOp {
    struct Expression* left;
    struct Expression* middle;
    struct Expression* right;
} TernOp;

typedef struct BinOp {
    struct Expression* left;
    struct Expression* right;
} BinOp;



typedef struct Assignment {
    struct Expression* left;
    
    unsigned int arrow_operator : 1; // else is equals assign
    unsigned int assigned_val_is_fn_literal : 1;
    union {
        struct StringStruct assigned_identifier; // this member is not needed in parser but in code_gen
        struct Expression*  assigned_expr;
        struct FnLiteral*   assigned_fn_literal;
    };
    
} Assignment;

typedef struct ValueOfOp {
    struct StringStruct variable_name;
} ValueOfOp;

typedef struct MemberAccessOP {
    struct Expression* parent;
    struct Expression* member;
} MemberAccessOP;

typedef struct Expression {
    enum ExpressionType expr_type;
    
    union {
        // literals
        long int              int_literal;
        char                  char_literal;
        struct StringStruct   string_literal;
        struct StringStruct   identifier_literal;
        //struct FnLiteral*   fn_ptr_literal;
        struct BinOp          bin_op;
        struct TernOp         tern_op;
        struct FnCall         fn_call;
        struct Assignment     assignment;
        struct ValueOfOp      val_of_op;
        struct MemberAccessOP member_access_op;
        
    };
    
    
} Expression;

typedef struct Statement {
    enum StatementType stmt_type;
    union {
        struct Expression* expr;
        struct ControlFlowIndicator cfi;
    };
} Statement;


typedef struct FunctionType {
    // DANGER: if amount_of_parameters is not zero-initialized, there is a double free in the if statement in the deepest
    //         loop void free_declaration_bucket(MainDeclarationBucket* declaration_bucket) in parser_commons.c
    int amount_of_fn_parameters;
    struct DataType* parameter_types;
    //
    struct DataType return_type;
} FunctionType;


typedef struct StructType {
    struct StringStruct struct_name;
    int amount_of_decls;
    struct Declaration** decl_ptr_arr;
} StructType;

typedef struct Declaration {
    struct StringStruct identifier;
    
    struct DataType type;
    
    unsigned int is_initialized : 1;
    //unsigned int variable_assigned : 1;
    union {
        //...
        //long int            init_int_val;
        //char                init_char_val;
        //struct StringStruct init_string;
        
        //...
        struct FnLiteral*   init_fn_ptr;
        struct Expression*  init_expr;
        //struct StringStruct init_variable;
    };
    
} Declaration;


typedef struct EntryPoint {
    struct VoidPtrArr decl_ptr_arr;
    struct VoidPtrArr stmt_ptr_arr;
    struct VoidPtrArr struct_type_ptr_arr;
    
    struct ImportList imports;
} EntryPoint;

typedef struct Allocators {
    struct MainFnLiteralBucket*   fn_literal_bucket;
    struct MainDeclarationBucket* declaration_bucket;
    struct MainStatementBucket*   statement_bucket;
    struct MainExpressionBucket*  expression_bucket;
    struct MainStructTypeBucket*  struct_type_bucket;
} Allocators;

typedef struct Program {
    struct EntryPoint* entry_point;
    
    struct Allocators  allocators;
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

// this should be visible to the programmer
typedef struct MainStatementBucket {
    struct StatementBucket* root_bucket;
} MainStatementBucket;

// this should be hidden from the programmer
typedef struct StatementBucket {
    int size, capacity;
    struct Statement* statements;
    struct StatementBucket* next_bucket;
    struct StatementBucket* prev_bucket;
} StatementBucket;

// this should be visible to the programmer
typedef struct MainExpressionBucket {
    struct ExpressionBucket* root_bucket;
} MainExpressionBucket;

// this should be hidden from the programmer
typedef struct ExpressionBucket {
    int size, capacity;
    struct Expression* expressions;
    struct ExpressionBucket* next_bucket;
    struct ExpressionBucket* prev_bucket;
} ExpressionBucket;

// this should be visible to the programmer
typedef struct MainStructTypeBucket {
    struct StructTypeBucket* root_bucket;
} MainStructTypeBucket;

// this should be hidden from the programmer
typedef struct StructTypeBucket {
    int size, capacity;
    struct StructType* struct_types;
    struct StructTypeBucket* next_bucket;
    struct StructTypeBucket* prev_bucket;
} StructTypeBucket;

/*----------------------------------------------------------------------------------------*/

/* FUNCTIONS */

struct Program* parse_tokens(struct TokenArr* tok_arr);

void parse_program_node(Program* program_ptr);
struct EntryPoint* parse_entry_point();
struct FnLiteral* parse_fn_literal(struct FnLiteral* surrounding_scope);
struct StructType* parse_struct_type();
struct Declaration* parse_parameter_declaration();
struct Declaration* parse_declaration(struct FnLiteral* surrounding_scope);
void parse_cfi(struct Statement* stmt);
struct Statement* parse_statement (struct FnLiteral* surrounding_scope);
void parse_imports(struct ImportList* import_list_ptr);

struct DataType parse_data_type();
struct FunctionType* parse_function_type();
struct FunctionType* parse_fn_literal_to_function_type(struct FnLiteral* fn_literal);

struct Expression* parse_expression();
struct Expression* parse_assignment(Expression* left_expr);
struct Expression* parse_if_else_expr();
struct Expression* parse_logic_expr();
struct Expression* parse_bool_term();
struct Expression* parse_cond_term();
struct Expression* parse_arith_term();
struct Expression* parse_value_of_term();
struct Expression* parse_factor();
struct Expression* parse_member_access_op(Expression* parent);
struct Expression* parse_fn_call_expr(Expression* fn_ptr);

struct Token* next_token();
struct Token* peek_token(int offset);
void eat_token(enum TokenType expected_type);
struct Token* cur_token();
int count_tokens_until_end_token_found(int token_to_count, int end_token);
int count_fn_parameters();
int count_struct_members();
enum TokenType get_tok_type(Token* tok);

void free_AST(struct Program* ast_root);

void init_ast_allocators(Program* program_ptr);

char* get_tok_name(Token* tok);

/*----------------------------------------------------------------------------------------*/

struct FnLiteral* add_fn_literal_to_bucket(struct FnLiteral* fn_to_add, struct MainFnLiteralBucket* fn_bucket);
void init_main_fn_literal_bucket(struct Program* program);
void free_fn_literal_bucket(MainFnLiteralBucket* fn_literal_bucket);

struct Declaration* add_declaration_to_bucket(struct Declaration* declaration_to_add, struct MainDeclarationBucket* declaration_bucket);
void init_main_declaration_bucket(struct Program* program);
void free_declaration_bucket(MainDeclarationBucket* declaration_bucket);

struct Statement* add_statement_to_bucket(struct Statement* statement_to_add, struct MainStatementBucket* statement_bucket);
void init_main_statement_bucket(struct Program* program);
void free_statement_bucket(MainStatementBucket* statement_bucket);

struct Expression* add_expression_to_bucket(struct Expression* expression_to_add, struct MainExpressionBucket* expression_bucket);
void init_main_expression_bucket(struct Program* program);
void free_expression_bucket(MainExpressionBucket* expression_bucket);

struct StructType* add_struct_type_to_bucket(struct StructType* struct_type_to_add, struct MainStructTypeBucket* struct_type_bucket);
void init_main_struct_type_bucket(struct Program* program);
void free_struct_type_bucket(MainStructTypeBucket* struct_type_bucket);

void init_void_ptr_arr(struct VoidPtrArr* void_ptr_arr);
void add_void_ptr_to_arr(struct VoidPtrArr* void_ptr_arr, void* ptr_to_add);
void free_void_ptr_arr(struct VoidPtrArr* void_ptr_arr);

#endif
