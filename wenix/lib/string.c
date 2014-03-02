/*
 * wenix/lib/string.c
 */

#include <string.h>
 
unsigned int strlen(const char * str)
{
    unsigned int i = 0;
    while (str[i])
        i++;
        
    return i;
}

unsigned int strcpy(char * dst, const char * src)
{
    unsigned int len = strlen(src);
    len++;
    
    memcpy(dst, src, len);
    return len;
}