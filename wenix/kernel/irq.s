SELECTOR_KERNEL_CS equ 8

extern exception_handler
extern spurious_irq

[section .text]
global	divide_error
global	debug
global	nmi
global	breakpoint
global	overflow
global	bounds
global	invalid_op
global	device_not_available
global	double_fault
global	coprocessor_segment_overrun
global	invalid_tss
global	segment_not_present
global	stack_segment
global	general_protection
global	page_fault
global	coprocessor_error
global  hwint00
global  hwint01
global  hwint02
global  hwint03
global  hwint04
global  hwint05
global  hwint06
global  hwint07
global  hwint08
global  hwint09
global  hwint10
global  hwint11
global  hwint12
global  hwint13
global  hwint14
global  hwint15

%macro hwint_master 1
    push (%1)
    call spurious_irq
    add esp, 4
    hlt
%endmacro

align 16
hwint00:
    iretd

align 16
hwint01:
    hwint_master 1

align 16
hwint02:
    hwint_master 2

align 16
hwint03:
    hwint_master 3

align 16
hwint04:
    hwint_master 4

align 16
hwint05:
    hwint_master 5

align 16
hwint06:
    hwint_master 6

align 16
hwint07:
    hwint_master 7

%macro hwint_slave 1
    push (%1)
    call spurious_irq
    add esp, 4
    hlt
%endmacro

align 16
hwint08:
    hwint_slave 8

align 16
hwint09:
    hwint_slave 9

align 16
hwint10:
    hwint_slave 10

align 16
hwint11:
    hwint_slave 11

align 16
hwint12:
    hwint_slave 12

align 16
hwint13:
    hwint_slave 13

align 16
hwint14:
    hwint_slave 14

align 16
hwint15:
    hwint_slave 15

divide_error:
    push 0xffffffff
    push 0
    jmp exception
debug:
    push 0xffffffff
    push 1
    jmp exception
nmi:
    push 0xffffffff
    push 2
    jmp exception
breakpoint:
    push 0xffffffff
    push 3
    jmp exception
overflow:
    push 0xffffffff
    push 4
    jmp exception
bounds:
    push 0xffffffff
    push 5
    jmp exception
invalid_op:
    push 0xffffffff
    push 6
    jmp exception
device_not_available:
    push 0xffffffff
    push 7
    jmp exception
double_fault:
    push 8
    jmp exception
coprocessor_segment_overrun:
    push 0xffffffff
    push 9
    jmp exception
invalid_tss:
    push 10
    jmp exception
segment_not_present:
    push 11
    jmp exception
stack_segment:
    push 12
    jmp exception
general_protection:
    push 13
    jmp exception
page_fault:
    push 14
    jmp exception
coprocessor_error:
    push 0xffffffff
    push 16
    jmp exception

exception:
    call exception_handler
    add  esp, 8
    hlt