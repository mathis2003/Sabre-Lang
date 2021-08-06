#ifndef STRING_COMMONS_H
#define STRING_COMMONS_H

#include <stdio.h>

typedef struct StringStruct {
    char* str_start;
    int str_length;
} StringStruct;


int str_equals_literal(struct StringStruct* str, char* str_literal);
int str_equals_str(struct StringStruct* str1, struct StringStruct* str2);
void print_str_struct(struct StringStruct* str);
int str_to_int(struct StringStruct* str);
char* str_to_c_str(struct StringStruct* str);
struct StringStruct c_str_to_str_struct(char* c_string);
struct StringStruct str_struct_cat_with_dot(struct StringStruct* str1, struct StringStruct* str2);
struct StringStruct str_struct_cat_with_arrow(struct StringStruct* str1, struct StringStruct* str2);

#endif
