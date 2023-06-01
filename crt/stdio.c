#include "stdio.h"

#include "stdint.h"
#include "stdarg.h"
#include "string.h"

int sprintf(char* str, const char* fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    

    while(*fmt != '\0'){

        if(*fmt == '%'){
            switch (*(++fmt))
            {
                case 'd':{
                    int32_t si = va_arg(ap, int32_t);
                }
                break;

                case 's':{
                    // String operand
                    const char* vstr = va_arg(ap, char*);
                    while(*vstr != '\0'){
                        vstr++;
                    }
                }
                break;
                default:  return -1;
            }
        }else{
            *str = *fmt;
        }
        fmt++;
    }

    va_end(ap);
}
// hey %s, welcome | "name" -> hey name, welcome
