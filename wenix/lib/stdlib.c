/*
 * wenix/lib/stdlib.c
 */

#include <stdlib.h>
//#include <assert.h>

static void do_div(unsigned int num, int base, int *quotient, int *remain)
{
    *quotient = num / base;
    *remain = num % base;
}

char * itoa(int value, char * buffer, int base)
{
    int num_count, quotient, remain;
    char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char tmp[36];
    char * string = buffer;
    
    //assert(8 == base || 10 == base || 16 == base);

    if (8 == base)
        *string++ = '0';
    else if (16 == base)
    {
        *string++ = '0';
        *string++ = 'x';
    }

    num_count = 0;
    if (0 == value)
        tmp[num_count++] = '0';
    else
    {
        /* 要注意这里的值都是逆序存放的 */
        while (value)
        {
            do_div((unsigned int)value, base, &quotient, &remain);
            tmp[num_count++] = digits[remain];
            value = quotient;
        }
    }

    while (num_count-- > 0)
        *string++ = tmp[num_count];

    return buffer;
}

int atoi(const char * s)
{
    int i = 0;

    while (isdigit(*s))
        i = i*10 + ((*s++) - '0');
    return i;
}
