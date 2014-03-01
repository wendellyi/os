; wenix/kernel/sys_call.s
;
; (C) 2014 Wendell Yi
;

; 处理硬件中断以及软件中断，比如时钟和系统调用

SELECTOR_OF_KERNEL equ 0x10

SIG_CHLD equ 17

REG_EAX equ 0x00
REG_EBX equ 0x04
REG_ECX equ 0x08
REG_EDX equ 0x0C
ORIG_EAX equ 0x10
REG_FS equ 0x14
REG_ES equ 0x18
REG_DS equ 0x1C
REG_EIP equ 0x20
REG_CS equ 0x24
REG_EFLAGS equ 0x28
REG_OLDESP equ 0x2C
REG_OLDSS equ 0x30

state equ 0
counter equ 4
priority equ 8
signal equ 12
sigaction equ 16
blocked equ (33*16)

sa_handler equ 0
sa_mask equ 4
sa_flags equ 8
sa_restorer equ 12

nr_system_calls equ 82
ENOSYS equ 82

[section .text]
global sys_call
global timer_interrupt

align 32
bad_sys_call:
    push -ENOSYS
    jmp ret_from_sys_call
    
align 32
coprocessor_error:
    push ds
    push es
    push fs
    push -1
    push edx
    push ecx
    push ebx
    push eax
    mov eax, SELECTOR_OF_KERNEL
    mov ds, ax
    mov es, ax
    mov eax, 0x17
    mov fs, ax
    push ret_from_sys_call
    jmp math_error
    
align 32
sys_call:
    push ds
    push es
    push fs
    push eax
    push edx
    push ecx
    push ebx
    mov edx, SELECTOR_OF_KERNEL
    mov ds, edx
    mov es, edx
    mov edx, 0x17
    mov fs, dx
    cmp nr_system_calls, eax
    jle bad_sys_call
    call [sys_call_table+4*eax]     ; 这个语法是错误的
    
align 32
timer_interrupt:
    ; push ds
    ; push es
    ; push fs
    ; push -1
    ; push edx
    ; push ecx
    ; push ebx
    ; push eax
    ; mov eax, SELECTOR_OF_KERNEL
    ; mov ds, ax
    ; mov es, ax
    ; mov eax, 0x17
    ; mov fs, ax
    ; inc jiffies
    ; mov al, 0x20
    ; outb 0x20, al
    ; mov do_timer
    