#ifndef _WENIX_STDLIB_H_
#define _WENIX_STDLIB_H_

/* 判定是否为数字 */
#define isdigit(c) ((c) >= '0' && (c) <= '9')

/* 只能完成八进制、十进制和十六进制的转换*/
char * itoa(int value, char * string, int base);

/* 只能完成十进制的转换*/
int atoi(const char * s);

#endif  /* _WENIX_STDLIB_H_ */
