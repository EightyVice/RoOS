#include "string.h"

void memcpy(void* dst, void* src, size_t count){
    char *d = dst;
    const char *s = src;
    for (size_t i = 0; i < count; i++)
        d[i] = s[i];   
}

void memset(void* ptr, int value, size_t num){
    char val = value;
    char* d = ptr;

    for (size_t i = 0; i < num; i++)
        d[i] = val;
}

void strcpy(char* dst, const char* src){
    while(*src != '\0'){
        *dst = *src;

        *dst++;
        *src++;
    }
    *dst = '\0';
}