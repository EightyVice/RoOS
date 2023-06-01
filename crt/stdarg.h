/*
    stdarg.h
    Handling Variadic Arguments
*/


typedef char* va_list;


#define _INTSIZEOF(n) ( (sizeof(n) + sizeof(int) - 1) / sizeof(int) * sizeof(int) )

#define va_start(ap, a)     ap = (va_list)&a + _INTSIZEOF(a)
#define va_arg(ap, t)       (*(t*)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define va_end(ap)          ((void)(ap = (va_list)0))