#ifndef STRING_COMMONS_H
#define STRING_COMMONS_H

typedef struct StringStruct {
    char* str_start;
    int str_length;
} StringStruct;


int str_equals_literal(struct StringStruct* str, char* str_literal);

#endif
