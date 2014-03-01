#ifndef _WENIX_SYSTEM_H_
#define _WENIX_SYSTEM_H_

#include <wenix/type.h>

void out_byte(uint16 port, uint8 value);
uint8 in_byte(uint16 port);

void init_8259a(void);

#endif /* _WENIX_SYSTEM_H_ */
