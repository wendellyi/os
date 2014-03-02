#ifndef _WENIX_STRING_H_
#define _WENIX_STRING_H_

#include <stdarg.h>

unsigned int strlen(const char * str);
unsigned int memcpy(void * dst, const void * src, unsigned int size);

int vsprintf(char * buffer, const char * fmt, va_list args);

#endif  /* _WENIX_STRING_H_ */
