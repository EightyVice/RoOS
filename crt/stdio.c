#include "stdio.h"

#include "stdint.h"
#include "stdarg.h"
#include "string.h"

int sprintf(char* str, const char* fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    
    int dest_pos = 0;

    while(*fmt != '\0'){

        if(*fmt == '%'){
            switch (*(++fmt))
            {
                case 'd':{
                    // unsigned 32bit integer
                    int32_t si = va_arg(ap, int32_t);   // 1202456 
                    char strint[11];
                    int digits = 0;
                    int temp = si;
                    int index = 0;
                    while(temp != 0){
                        strint[index] = temp % 10;
                        temp /= 10;
                        index++;
                        digits++;
                    }
                    
                    for (size_t i = digits - 1; i > 0; i--)
                    {
                        *(str + dest_pos) = strint[i] + '0';
                        dest_pos++;
                    }
                    

                    // convert to string
                    
                }
                break;

                case 's':{
                    // String operand
                    const char* vstr = va_arg(ap, char*);
                    while(*vstr != '\0'){
                       *(str + dest_pos) = *vstr;
                       vstr++;
                       dest_pos++;
                    }
                    *fmt++;
                    continue;
                }
                break;
                default:  return -1;
            }
        }else{
            *(str + dest_pos) = *fmt;
        }
        fmt++;
        dest_pos++;
    }

    va_end(ap);
}
// hey %s, welcome | "name" -> hey name, welcome
