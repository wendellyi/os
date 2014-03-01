/*
 * wenix/kernel/printk.c
 *
 * (C) 2014 Wendell Yi
 */

#include <stdarg.h>
#include <stddef.h>
#include <wenix/kernel.h>

static char buffer[1024];

extern int vsprintf(char * buffer, const char * fmt, va_list args);

int printk(const char * fmt, ...)
{
    va_list args;
    int i;
    
    va_start(args, fmt);
    i = vsprintf(buffer, fmt, args);
    va_end(args);
    console_print(buffer);
    return i;
}

