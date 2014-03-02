/*
 * wenix/include/wenix/system.h
 */
#ifndef _WENIX_SYSTEM_H_
#define _WENIX_SYSTEM_H_

#include <wenix/type.h>

void print_char(char ch, uint8 color);
void print_string(const char * str);
void print_color_string(const char * str, uint8 color);
int get_display_position(void);
void set_display_position(int position);
unsigned int vgacpy(void * dst, void * src, unsigned int size);
int printk(const char * fmt, ...);
void init_8259a(void);

#endif /* _WENIX_SYSTEM_H_ */
