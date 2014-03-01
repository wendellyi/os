#include <asm/system.h>
#include <wenix/const.h>

void init_8259a(void)
{
    out_byte(INT_M_CTL, 0x11);
    out_byte(INT_S_CTL, 0x11);
    out_byte(INT_M_CTL_MASK, INT_VECTOR_IRQ0);
    out_byte(INT_S_CTL_MASK, INT_VECTOR_IRQ8);
    out_byte(INT_M_CTL_MASK, 0x04);
    out_byte(INT_S_CTL_MASK, 0x02);
    out_byte(INT_M_CTL_MASK, 0x01);
    out_byte(INT_S_CTL_MASK, 0x01);
    out_byte(INT_M_CTL_MASK, 0xff);
    out_byte(INT_S_CTL_MASK, 0xff);
}
