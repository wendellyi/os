#include <wenix/type.h>
#include <wenix/const.h>

void* memcpy(void* dst, void* src, int size);
void* display_string(char* string);

/*
 * 下面函数的功能是将原来的gdt复制到新的位置
 * 然后同步将gdt_ptr中的长度与基址同时也进行更新
 */
void cstart(void)
{
    display_string("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
                   "-- cstart begins --\n");

    /* 将gdt复制到新的地方 */
    memcpy(&gdt,
           (void *)(*((u32*)(&gdt_ptr[2]))),
           *((u16*)(&gdt_ptr[0]))+1);

    u16* p_gdt_limit = (u16*)(&gdt_ptr[0]);
    u32* p_gdt_base  = (u32*)(&gdt_ptr[2]);
    *p_gdt_limit = GDT_SIZE * sizeof(struct DESCRIPTOR) -1;
    *p_gdt_base  = (u32)&gdt;

    u16* p_idt_limit = (u16*)(&idt_ptr[0]);
    u32* p_idt_base = (u32*)(&idt_ptr[2]);
    *p_idt_limit = IDT_SIZE * sizeof(struct GATE);
    *p_idt_base = (u32)&idt;

    init_prot();

    display_string("-- cstart ends --\n");
}
