/*
 * wenix/kernel/printk.c
 *
 * (C) 2014 Wendell Yi
 */

#include <string.h>
#include <wenix/system.h>

static char buffer[1024];

int printk(const char * fmt, ...)
{
    va_list args;
    int i;
    
    va_start(args, fmt);
    i = vsprintf(buffer, fmt, args);
    va_end(args);
    print_string(buffer);
    return i;
}

