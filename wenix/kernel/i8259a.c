#include <wenix/io.h>
#include <wenix/const.h>

void init_8259a(void)
{
    outb_p(INT_M_CTL, 0x11);
    outb_p(INT_S_CTL, 0x11);
    outb_p(INT_M_CTL_MASK, INT_VECTOR_IRQ0);
    outb_p(INT_S_CTL_MASK, INT_VECTOR_IRQ8);
    outb_p(INT_M_CTL_MASK, 0x04);
    outb_p(INT_S_CTL_MASK, 0x02);
    outb_p(INT_M_CTL_MASK, 0x01);
    outb_p(INT_S_CTL_MASK, 0x01);
    outb_p(INT_M_CTL_MASK, 0xff);
    outb_p(INT_S_CTL_MASK, 0xff);
}
