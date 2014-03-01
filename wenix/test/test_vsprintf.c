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
    if (type & LEFT)            /* LEFT������ZEROPAD */
        type &= ~ZEROPAD;
    if (base < 2 || base > 36)  /* ���Ʋ����Ϲ淶 */
        return 0;

    pading = (type & ZEROPAD) ? '0' : ' '; /* �趨����ַ� */
    if (type & SIGN && num < 0)
    {
        sign = '-';
        num = -num;
    }
    else
        sign = (type & PLUS) ? '+' : ((type&SPACE)?' ':0); /* �趨��ֵ�ķ��� */

    if (sign) size--;                   /* �з������Ե�һλ */

    if (type & SPECIAL)
    {
        if (16 == base) size -= 2; /* ��ǰ��0x�۳���λ */
        else if (8 == base) size--; /* ��ǰ��0�۳�һλ */
    }

    /* ��ʼת����ֵ�� */
    num_count = 0;
    if (0 == num)
        tmp[num_count++] = '0';
    else
    {
        /* Ҫע�������ֵ���������ŵ� */
        while (num)
        {
            do_div((unsigned int)num, base, &quotient, &remain);
            tmp[num_count++] = digits[remain];
            num = quotient;
        }
    }

    /* λ�����ھ�����λ������������� */
    if (num_count > precision)
        precision = num_count;

    /* size��Ҫ�۳���ֵ��λ���� */
    size -= precision;

    /* ��û��������0��䣬ǰ���һֱ���ո� */
    if (!(type & (ZEROPAD|LEFT)))
        while (size-- > 0)
            *str++ = ' ';

    /* ������λ */
    if (sign)
        *str++ = sign;

    /* ���ǰ���ַ� */
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

    /* ���û������룬ʹ������ַ� */
    if (!(type & LEFT))
        while (size-- > 0) *str++ = pading;

    while (num_count < precision--) /* ������ȴ�����ֵ��λ����Ҫ���0 */
        *str++ = '0';
    while (num_count-- > 0)             /* ���������ֵ */
        *str++ = tmp[num_count];
    while (size-- > 0)          /* ������Ҷ�����ôʹ�ÿո񲹳� */
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
