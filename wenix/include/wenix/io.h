/*
 * wenix/include/io.h
 */
 
#include <wenix/type.h>
 
void outb(uint16 port, uint8 value);
uint8 inb(uint16 port);
void outb_p(uint16 port, uint8 value);
uint8 inb_p(uint16 port);
