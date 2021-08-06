#include <stdlib.h>
#include "string_commons.h"

int str_equals_literal(struct StringStruct* str, char* str_literal) {
    int i;
    for (i = 0; i < str->str_length && str_literal[i] != '\0'; i++) {
        if (str->str_start[i] != str_literal[i]) return 0;
    }
    
    // have we gotten to the end of BOTH strings? if not -> return false
    if ((i != str->str_length) || (str_literal[i] != '\0')) return 0;
    
    return 1;
}

void print_str_struct(struct StringStruct* str) {
    for (int i = 0; i < str->str_length; i++)
        printf("%c", str->str_start[i]);
}

int str_to_int(struct StringStruct* str) {
    int ret_val = 0;
    
    for (int i = 0; i < str->str_length; i++) {
        ret_val *= 10;
        ret_val += str->str_start[i] - '0';
    }
    
    return ret_val;
}

char* str_to_c_str(struct StringStruct* str) {
    char* ret_str = malloc((str->str_length + 1) * sizeof(char));
    for (int i = 0; i < str->str_length; i++)
        ret_str[i] = (str->str_start)[i];
    ret_str[str->str_length] = '\0';
    
    return ret_str;
}

struct StringStruct c_str_to_str_struct(char* c_string) {
    int length = 0;
    
    for (int i = 0; c_string[i] != '\0'; i++) {
        length++;
    }
    
    struct StringStruct ret_str;
    ret_str.str_length = length;
    ret_str.str_start = malloc(ret_str.str_length * sizeof(char));
    
    for (int i = 0; i < ret_str.str_length; i++) {
        ret_str.str_start[i] = c_string[i];
    }
    
    return ret_str;
}
