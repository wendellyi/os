; wenix/kernel/asm.s
;
; 这里的中断处理函数都是些异常处理函数，一般都会造成死机

; 导入外部函数
extern do_divide_error
extern do_break_point
extern do_nmi
extern do_overflow
extern do_bounds
extern do_invalid_op
extern do_coprocessor_segment_overrun    
extern do_reserved
extern do_irq13
extern do_double_fault
extern do_invalid_tss
extern do_segment_not_present
extern do_stack_segment
extern do_general_protection
extern do_alignment_check

[section .text]
; 导出符号，用于设置idt
global divide_error
global debug
global nmi 
global break_point
global overflow
global bounds
global invalid_op
global coprocessor_segment_overrun
global reserved
global irq13    
global double_fault
global invalid_tss
global segment_not_present
global stack_segment
global general_protection
global alignment_check

divide_error:
    push do_divide_error
    jmp no_error_code
    
debug:
    push do_break_point
    jmp no_error_code
    
nmi:
    push do_nmi
    jmp no_error_code
    
break_point:
    push do_break_point
    jmp no_error_code
    
overflow:
    push do_overflow
    jmp no_error_code
    
bounds:
    push do_bounds
    jmp no_error_code
    
invalid_op:
    push do_invalid_op
    jmp no_error_code
    
coprocessor_segment_overrun:
    push do_coprocessor_segment_overrun
    jmp no_error_code
    
reserved:
    push do_reserved
    jmp no_error_code
    
; 这个中断后面再说
irq13:
    push do_irq13
    jmp no_error_code
    
double_fault:
    push do_double_fault
    jmp error_code
    
invalid_tss:
	push do_invalid_tss
	jmp error_code

segment_not_present:
	push do_segment_not_present
	jmp error_code

stack_segment:
	push do_stack_segment
	jmp error_code

general_protection:
	push do_general_protection
	jmp error_code

alignment_check:
	push do_alignment_check
	jmp error_code
    
no_error_code:
    xchg eax, [esp]
    push ebx
    push ecx
    push edx
    push edi
    push esi
    push ebp
    push ds
    push es
    push fs
    push 0                  ; 错误码为0，表示没有错误码
    lea edx, [esp+44]
    push edx
    mov edx, 10
    mov ds, dx
    mov es, dx
    mov fs, dx
    call [eax]
    add esp, 8
    pop fs
    pop es
    pop ds
    pop ebp
    pop esi
    pop edi
    pop edx
    pop ecx
    pop ebx
    pop eax
    iret
    
error_code:
    xchg eax, [esp+4]       ; 错误码放在了eax中
    xchg ebx, [esp]         ; 调用函数地址放在了ebx中
    push ecx
    push edx
    push edi
    push esi
    push ebp
    push ds
    push es
    push fs
    push eax                ; 错误码放在了eax
    lea edx, [esp+44]
    push eax
    mov eax, 10
    mov ds, ax
    mov es, ax
    mov fs, ax
    call [ebx]
    add esp, 8
    pop fs
    pop es
    pop ds
    pop ebp
    pop esi
    pop edi
    pop edx
    pop ecx
    pop ebx
    pop eax
    iret