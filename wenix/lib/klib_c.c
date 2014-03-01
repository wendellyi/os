#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "global.h"

char * itoa(char * string, int number)
{
    char * p = string;
    char ch;
    int i;
    int flag = 0;

    *(p++) = '0';
    *(p++) = 'x';

    if (0 == number)
        *(p++) = '0';
    else
    {
        for (i = 28; i >= 0; i -= 4)
        {
            ch = (num >> i) & 0x0f;
            if (flag || (ch > 0))
            {
                flag = 1;
                ch += '0';
                if (ch > '9')
                    ch += 7;

                *(p++) = ch
            }
        }
    }

    *p = 0;
    return string;
}

void display_int(int i)
{
    char output[16];
    itoa(output, i);
    display_string(output);
}
