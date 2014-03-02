;; 需要使用汇编代码编写的一些函数
[section .data]
display_position: dd 0

;; 导出符号
[section .text]
global outb
global inb
global outb_p
global inb_p
global print_char
global get_display_position
global set_display_position
global memcpy
global vgacpy

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 用于io操作的函数
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 注意下面的四个函数没有使用ebp，与常见的不太一样
;; void outb(uint16 port, uint8 value);
outb:
    mov edx, [esp+4]            ; port
    mov al, [esp+8]             ; value
    out dx, al
    ret

;; uint8 inb(uint16 port);
inb:
    mov edx, [esp+4]            ; port
    xor eax, eax                ; 返回值保存在eax中
    in al, dx
    ret
    
;; 带延迟的outb
;; void outb_p(uint16 port, uint8 value);
outb_p:
    mov edx, [esp+4]            ; port
    mov al, [esp+8]             ; value
    nop                          ; 一点点延迟
    nop
    ret
    
;; 带延迟的inb
;; uint8 inb(uint16 port);
inb_p:
    mov edx, [esp+4]            ; port
    xor eax, eax                ; 返回值保存在eax中
    in al, dx
    nop                         ; 一点点延迟
    nop
    ret
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 用于字符操作的函数
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 功能：显示字符
;; void print_char(char ch, uint8 color);
print_char:
    push ebp
    mov ebp, esp
    
    mov al, [ebp+8]             ; ch
    mov ah, [ebp+12]            ; color
    mov edi, [display_position]
    cmp al, 0x0a                ; 判断是否是回车
    jz .2
.1:
    mov [gs:edi], ax
    add edi, 2
.2:
    mov esp, ebp
    pop ebp
    ret
    
;; 获取显示的位置
;; int get_display_position(void);
get_display_position:
    mov eax, [display_position]
    ret
    
;; 设置显示的位置
;; void set_display_position(int position);
set_display_position:
    mov eax, [esp+4]
    mov dword [display_position], eax
    ret
    
;; 复制内存的内容，前提是ds和es是一致的
;; unsigned int memcpy(void * es:dst, void * ds:src, unsigned int size);
memcpy:
    push ebp
    mov ebp, esp

    push esi
    push edi
    push ecx

    mov edi, [ebp+8]            ; dst
    mov esi, [ebp+12]           ; src
    mov ecx, [ebp+16]           ; size
.1:
    cmp ecx, 0
    jz .2

    mov al, [ds:esi]
    inc esi

    mov byte [es:edi], al
    inc edi
    dec ecx
    jmp .1
.2:
    pop ecx
    pop edi
    pop esi
    mov eax, [ebp+16]
    mov esp, ebp
    pop ebp
    ret
    
;; 复制内存的内容，前提是ds和es是一致的
;; unsigned int vgacpy(void * gs:dst, void * gs:src, unsigned int size);
vgacpy:
    push ebp
    mov ebp, esp

    push esi
    push edi
    push ecx

    mov edi, [ebp+8]            ; dst
    mov esi, [ebp+12]           ; src
    mov ecx, [ebp+16]           ; size
.1:
    cmp ecx, 0
    jz .2

    mov al, [gs:esi]
    inc esi

    mov byte [gs:edi], al
    inc edi
    dec ecx
    jmp .1
.2:
    pop ecx
    pop edi
    pop esi
    mov eax, [ebp+16]
    mov esp, ebp
    pop ebp
    ret