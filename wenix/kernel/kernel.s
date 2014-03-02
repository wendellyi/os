SELECTOR_KERNEL_CS equ 8

extern init
extern gdt_ptr
extern idt_ptr

[section .bss]
stack_space resb 2*1024
bottom_of_stack:

[section .text]
global _start                           ; 导出_start

_start:
    mov esp, bottom_of_stack
    sgdt [gdt_ptr]                      ; 由此可见数组的名称在编译器看来就是内存块的首地址
    call init                           ; 初始化一些必要的内容
    lgdt [gdt_ptr]                      ; 重新载入gdt
    lidt [idt_ptr]

    jmp SELECTOR_KERNEL_CS:cs_flush     ; 这里的意图是强制刷新cs中的值

cs_flush:
    ud2
    push 0                              ; 下面即将初始化eflags
    popfd                               ; 弹出eflags
    hlt
    
