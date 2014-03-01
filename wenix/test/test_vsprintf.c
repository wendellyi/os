#include <stdio.h>
#include <string.h>
#include <assert.h>

#define ZEROPAD     1
#define SIGN        2
#define PLUS        4
#define SPACE       8
#define LEFT        16
#define SPECIAL     32
#define SMALL       64

void do_div(unsigned int num, int base, int *quotient, int *remain)
{
    *quotient = num / base;
    *remain = num % base;
}

static char * number(
    char * str, int num, int base, int size, int precision, int type)
{
    int quotient, remain;
    char pading, sign, tmp[36];
    const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int num_count;

    if (type & SMALL)
        digits = "0123456789abcdefghijklmnopqrstuvwxyz";
    if (type & LEFT)            /* LEFT优先于ZEROPAD */
        type &= ~ZEROPAD;
    if (base < 2 || base > 36)  /* 进制不符合规范 */
        return 0;

    pading = (type & ZEROPAD) ? '0' : ' '; /* 设定填充字符 */
    if (type & SIGN && num < 0)
    {
        sign = '-';
        num = -num;
    }
    else
        sign = (type & PLUS) ? '+' : ((type&SPACE)?' ':0); /* 设定数值的符号 */

    if (sign) size--;                   /* 有符号填充吃掉一位 */

    if (type & SPECIAL)
    {
        if (16 == base) size -= 2; /* 有前导0x扣除两位 */
        else if (8 == base) size--; /* 有前导0扣除一位 */
    }

    /* 开始转换数值了 */
    num_count = 0;
    if (0 == num)
        tmp[num_count++] = '0';
    else
    {
        /* 要注意这里的值都是逆序存放的 */
        while (num)
        {
            do_div((unsigned int)num, base, &quotient, &remain);
            tmp[num_count++] = digits[remain];
            num = quotient;
        }
    }

    /* 位数大于精度则位数起决定性作用 */
    if (num_count > precision)
        precision = num_count;

    /* size又要扣除数值的位数了 */
    size -= precision;

    /* 若没有左对齐和0填充，前面会一直补空格 */
    if (!(type & (ZEROPAD|LEFT)))
        while (size-- > 0)
            *str++ = ' ';

    /* 填充符号位 */
    if (sign)
        *str++ = sign;

    /* 填充前导字符 */
    if (type & SPECIAL)
    {
        if (8 == base)
            *str++ = '0';
        else if (16 == base)
        {
            *str++ = '0';
            *str++ = digits[33];
        }
    }

    /* 如果没有左对齐，使用填充字符 */
    if (!(type & LEFT))
        while (size-- > 0) *str++ = pading;

    while (num_count < precision--) /* 如果精度大于数值的位数需要填充0 */
        *str++ = '0';
    while (num_count-- > 0)             /* 倒序填充数值 */
        *str++ = tmp[num_count];
    while (size-- > 0)          /* 如果是右对齐那么使用空格补充 */
        *str++ = ' ';
    return str;
}

void test_number(void)
{
    char buffer[128];
    char * str;
    str = number(buffer, 128, 16, 10, 4, SMALL | SPECIAL);
    *str = '\0';
    assert(0 == strcmp(buffer, "    0x0080"));

    str = number(buffer, -128, 16, 10, 4, SMALL | SPECIAL | SIGN);
    *str = '\0';
    assert(0 == strcmp(buffer, "   -0x0080"));

    str = number(buffer, 128, 16, 10, 4, SPECIAL);
    *str = '\0';
    assert(0 == strcmp(buffer, "    0X0080"));

    str = number(buffer, 128, 16, 10, 4, SMALL);
    *str = '\0';
    assert(0 == strcmp(buffer, "      0080"));

    str = number(buffer, -128, 16, 10, 4, SMALL | SPECIAL);
    *str = '\0';
    assert(0 == strcmp(buffer, "0xffffff80"));

    str = number(buffer, 128, 16, 10, 4, SMALL | SPECIAL);
    *str = '\0';
    assert(0 == strcmp(buffer, "    0x0080"));
}

void test_vsprintf(void)
{
    test_number();
}
