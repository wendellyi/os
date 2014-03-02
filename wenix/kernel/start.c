#include <wenix/type.h>
#include <wenix/const.h>
#include <wenix/system.h>
#include <wenix/intrrupt.h>
#include <string.h>

extern struct desc_t gdt[NR_GDT];
extern struct gate_t idt[NR_IDT];
extern uint8 gdt_ptr[6];
extern uint8 idt_ptr[6];

/*
 * 下面函数的功能是将原来的gdt复制到新的位置
 * 然后同步将gdt_ptr中的长度与基址同时也进行更新
 */
void init(void)
{
    print_string("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
                   "-- cstart begins --\n");
                                 
    /* 将gdt复制到新的地方 */
    memcpy(&gdt, (void *)(long)(*((uint32*)(&gdt_ptr[2]))), *((uint16*)(&gdt_ptr[0]))+1);
    uint16* p_gdt_limit = (uint16 *)(&gdt_ptr[0]);
    uint32* p_gdt_base  = (uint32 *)(&gdt_ptr[2]);
    *p_gdt_limit = NR_GDT * sizeof(struct desc_t) -1;
    *p_gdt_base = (uint32)(long)&gdt;

    uint16* p_idt_limit = (uint16 *)(&idt_ptr[0]);
    uint32* p_idt_base = (uint32 *)(&idt_ptr[2]);
    *p_idt_limit = NR_IDT * sizeof(struct gate_t);
    *p_idt_base = (uint32)(long)&idt;
    int_init();

    print_string("-- cstart ends --\n");
}
