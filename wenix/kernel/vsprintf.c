/*
 * wenix/kernel/vsprintf.c
 */

#include <stdarg.h>
#include <string.h>

#define isdigit(c) ((c) >= '0' && (c) <= '9')

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

/* 将字符串转换成整数，并且移动索引，这就是skip的意思 */
static int skip_atoi(const char ** s)
{
    int i = 0;

    while (isdigit(**s))
        i = i*10 + (*((*s)++) - '0');

    return i;
}

/*
 * 这个函数的功能是将整数转换成相应条件的字符串
 * str 字符串缓冲区
 * num 整数
 * base 进制
 * size 字符串长度
 * precision 精度
 * type 各种标记，比如左对齐、符号等
 */
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

int vsprintf(char * buffer, const char * fmt, va_list args)
{
    char * str;
    int flags;
    int field_width;
    int precision;
    int qualifier = -1;

    for (str = buffer; *fmt; fmt++)
    {
        /* 判定是否是格式化的开始 */
        if (*fmt != '%')
        {
            *str = *fmt;
            str++;
            continue;
        }

        /* 解析flags */
        fmt++;                  /* 跳过% */
        flags = 0;
        while (' ' == *fmt || '+' == *fmt || '-' == *fmt || '#' == *fmt || '0' == *fmt)
        {
            switch (*fmt)
            {
            case ' ':
                flags |= SPACE;
                break;

            case '+':
                flags |= PLUS;
                break;

            case '-':
                flags |= LEFT;
                break;

            case '#':
                flags |= SPECIAL;
                break;

            case '0':
                flags |= ZEROPAD;
                break;
            }

            fmt++;
        } /* end while */

        /* 获取宽度 */
        field_width = 0;
        if (isdigit(*fmt))      /* 宽度使用十进制数值直接给出 */
            field_width = skip_atoi(&fmt);
        else if ('*' == *fmt)   /* 下一个参数指定宽度 */
        {
            field_width = va_arg(args, int);
            if (field_width < 0)
            {
                field_width = -field_width;
                flags |= LEFT;
            }

            fmt++;
        }

        /* 获取精度 */
        precision = 0;
        if ('.' == *fmt)
        {
            fmt++;
            if (isdigit(*fmt))  /* 精度直接给出 */
                precision = skip_atoi(&fmt);
            else if ('*' == *fmt) /* 精度在下个参数中给出 */
            {
                precision = va_arg(args, int);
                fmt++;
            }

            if (precision < 0)
                precision = 0;
        }

        /* 获取转换 */
        qualifier = 0;
        if ('h' == *fmt || 'l' == *fmt || 'L' == *fmt)
        {
            qualifier = *fmt;
            fmt++;
        }

        switch (*fmt)
        {
        case 'c':
            if (!(flags & LEFT)) /* 没有左对齐 */
                while (--field_width > 0)
                    *str++ = ' ';

            *str++ = (unsigned char)va_arg(args, int);
            while (--field_width > 0) /* 左对齐 */
                *str++ = ' ';
            break;

        case 's':
            s = va_arg(args, char *);
            len = strlen(s);
            if (precision < 0)
                precision = len;
            else if (len > precision)
                len = precision;

            if (!(flags & LEFT))
                while (len < field_width--)
                    *str++ = ' ';
            for (i = 0; i < len; ++i)
                *str++ = *s++;
            while (len < field_width--)
                *str++ = ' ';
            break;

        case 'o':
            str = number
            break;

        case 'p':
            break;

        case 'x':
            break;

        case 'X':
            break;

        case 'd':
            break;

        case 'i':
            break;

        case 'u':
            break;

        case 'n':
            break;

        default:
            break;
        } /* end switch */
    } /* end for */

    *str = '\0';
    return str-buffer;
}
