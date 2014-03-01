SELECTOR_KERNEL_CS equ 8

extern cstart
extern gdt_ptr

[section .bss]
stack_space resb 2*1024
bottom_of_stack:

[section .text]

global _start                   ; 导出_start

_start:
    mov esp, bottom_of_stack
    sgdt [gdt_ptr]              ; 由此可见数组的名称在编译器看来就是内存块的首地址
    call cstart
    lgdt [gdt_ptr]

    jmp SELECTOR_KERNEL_CS:csinit ; 这里的意图是强制刷新cs中的值

csinit:
    push 0                      ; 下面即将初始化eflags
    popfd                       ; 弹出eflags
    hlt