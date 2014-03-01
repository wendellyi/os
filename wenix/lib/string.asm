;; 与字符串处理有关的库函数

;; 下面是导出的符号
[section .text]
global memcpy

;; void memcpy(void* es:dst, void* ds:src, int size);
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
    mov esp, ebp
    pop ebp
    ret