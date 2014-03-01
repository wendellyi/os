[section .data]
display_position dd 0

[section .text]
global display_string
global out_byte
global in_byte

;; 功能：显示字符串
;; void display_string(char * string);
display_string:
    push ebp
    mov ebp, esp

    mov esi, [ebp + 8]          ; 第一个参数，也就是string
    mov edi, [display_position]
    mov ah, 0x0f
.1:
    lodsb
    test al, al                 ; 判断是否遇到空字符
    jz .2
    cmp al, 0x0a                ; 是回车吗
    jnz .3
    push eax
    mov eax, edi
    mov bl, 160
    div bl
    and eax, 0xff
    inc eax
    mov bl, 160
    mul bl
    mov edi, eax
    pop eax
    jmp .1
.3:
    mov [gs:edi], ax
    add edi, 2
    jmp .1

.2:
    mov [display_position], edi
    mov esp, ebp                ;清理栈
    pop ebp
    ret

;; void display_color_string(char* string, int color);
display_color_string:
    push ebp
    mov ebp, esp

    mov esi, [ebp+8]            ; string
    mov edi, [display_position]
    mov ah, [ebp+12]            ; color

.1:
    lodsb
    test al, al                 ; 测试终止字符
    jz .2
    cmp al, 0x0a
    jnz .3
    push eax
    mov eax, edi
    mov bl, 160
    div bl
    and eax, 0xff
    inc eax
    mov bl, 160
    mul bl
    mov edi, eax
    pop eax
    jmp .1
.3:
    mov [gs:edi], ax
    add edi, 2
    jmp .1
.2:
    mov [display_position], edi

    mov esp, ebp
    pop ebp
    ret

;; 注意下面这两个函数没有使用ebp，与常见的不太一样
;; void out_byte(u16 port, u8 value);
out_byte:
    mov edx, [esp+4]            ; port
    mov al, [esp+4+4]           ; value
    out dx, al
    nop                         ; 一点点延迟
    nop
    ret

;; u8 in_byte(u16 port);
in_byte:
    mov edx, [esp+4]            ; port
    xor eax, eax                ; 返回值默认保存在eax中
    in al, dx
    nop                         ; 一点点延迟
    nop
    ret
