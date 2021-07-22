
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
