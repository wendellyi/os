/*
 * wenix/kernel/global.c
 *
 * (C) 2014 Wendell Yi
 */
 
/*
 * 定义了一些全局变量
 */
 
#include <wenix/type.h>
#include <wenix/const.h>

struct desc_t gdt[NR_GDT];              /* gdt 数组 */
struct gate_t idt[NR_IDT];              /* idt 数组 */
uint8 gdt_ptr[6];
uint8 idt_ptr[6];
