#include <wenix/system.h>

/* 异常处理函数，向屏幕输出重要寄存器的值 */
void exception_handler(
    int irq, int err_code, int eip, int cs, int eflags)
{
    int i;
    int text_color = 0x74;      /* 灰底红字 */

	char * err_msg[] = {
        "#DE Divide Error",
        "#DB RESERVED",
        "--  NMI Interrupt",
        "#BP Breakpoint",
        "#OF Overflow",
        "#BR BOUND Range Exceeded",
        "#UD Invalid Opcode (Undefined Opcode)",
        "#NM Device Not Available (No Math Coprocessor)",
        "#DF Double Fault",
        "    Coprocessor Segment Overrun (reserved)",
        "#TS Invalid TSS",
        "#NP Segment Not Present",
        "#SS Stack-Segment Fault",
        "#GP General Protection",
        "#PF Page Fault",
        "--  (Intel reserved. Do not use.)",
        "#MF x87 FPU Floating-Point Error (Math Fault)",
        "#AC Alignment Check",
        "#MC Machine Check",
        "#XF SIMD Floating-Point Exception"
    };

	/* 通过打印空格的方式清空屏幕 */
	set_display_position(0);
	for(i = 0; i < 80*5; i++)
        print_string(" ");

    set_display_position(0);
	print_color_string("Exception! --> ", text_color);
	print_color_string(err_msg[irq], text_color);
    printk("EFLAGS: %x\n", eflags);
    printk("CS: %x\n", cs);
    printk("EIP: %x\n", eip);
	
    /* 如果是错误码将不等于我们填充的值 */
	if(err_code != 0xFFFFFFFF)
        printk("Error Code: %x\n\n", err_code);
}


void spurious_irq(int irq)
{
    printk("%s: %#x", __FUNCTION__, irq);
}