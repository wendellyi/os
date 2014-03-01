#ifndef _WENIX_STDARG_H_
#define _WENIX_STDARG_H_

typedef char * va_list;

/* 下面这个其实使用了(((x+(n-1))/n)*n)计算最少占用的公式
 * 其实通过计算可以知道，宏计算出来的值最少为4 */
#define __va_rounded_size(type) \
    (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

/* 计算last_arg之后第一个参数的地址 */
#define va_start(arg_ptr, last_arg) \
    (arg_ptr = ((char *)&(last_arg) + __va_rounded_size(last_arg)))

/* 逗号为顺序点，下面宏的作用是将参数指针指向下一个参数，并且返回当前参数的值
 * 用得很赞 */
#define va_arg(arg_ptr, type) \
    (arg_ptr += __va_rounded_size(type), *((type *)(arg_ptr - __va_rounded_size(type))))

#define va_end(arg_ptr)

#endif  /* _WENIX_STDARG_H_ */
