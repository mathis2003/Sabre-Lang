#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void die(const char* err_msg, ...) {
    printf("\n");
    
    va_list args;
    va_start(args, err_msg);
    
    for (int i = 0; err_msg[i] != '\0'; i++) {
        if (err_msg[i] == '%') {
            switch (err_msg[i+1]) {
                case 'c': {
                    int a = va_arg(args, int);
                    printf("%c", a);
                    i++;
                    break;
                }
                case 'd': {
                    int a = va_arg(args, int);
                    printf("%d", a);
                    i++;
                    break;
                }
                case 's': {
                    char* a = va_arg(args, char*);
                    printf("%s", a);
                    i++;
                    break;
                }
                default: {
                    putchar(err_msg[i]);
                    break;
                }
                    
            }
        } else putchar(err_msg[i]);
    }
    
    va_end(args);
    
    exit(1);
}

