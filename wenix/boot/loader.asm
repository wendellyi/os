;; loader的主要功能是：
;; 1.将系统的内存信息在实模式下读取出来
;; 2.在实模式下将kernel加载到指定的地址中
;; 3.进入保护模式
;; 4.将内存的信息显示出来
;; 5.开启分页
;; 6.将kernel的代码移动到正确的地方，然后跳转到kernel中

org 0x0100                  ; 默认的偏移是必须的
jmp LABEL_START

%include "protect.inc"      ; 与保护模式相关的宏
%include "loader.inc"       ; 与loader相关的宏

LABEL_GDT:              DESCRIPTOR 0, 0, 0
LABEL_DESC_FLAT_CODE:   DESCRIPTOR 0, 0xffffff, (DA_CR|DA_32|DA_LIMIT_4K)
LABEL_DESC_FLAT_RW:     DESCRIPTOR 0, 0xffffff, (DA_DRW|DA_32|DA_LIMIT_4K)
LABEL_DESC_VIDEO:       DESCRIPTOR 0xb8000, 0xffff, (DA_DRW|DA_DPL3)

gdt_len equ $-LABEL_GDT         ; gdt长度
gdt_ptr dw gdt_len-1            ; gdt段界限
        dd logic_addr_of_loader+LABEL_GDT ; 段基址
        
selector_flat_code equ LABEL_DESC_FLAT_CODE-LABEL_GDT
selector_flat_rw equ LABEL_DESC_FLAT_RW-LABEL_GDT
selector_video equ LABEL_DESC_VIDEO-LABEL_GDT

LABEL_START:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, rm_bottom_of_stack
    
    mov bp, loading_msg
    call print_msg

    mov ebx, 0
    mov di, _mem_chk_buffer
.memchk_loop:
    mov eax, 0xe820
    mov ecx, ard_struct_size
    mov edx, 0x534d4150
    int 0x15
    jc .memchk_failed
    add di, ard_struct_size
    inc dword [_mem_chk_result]
    cmp ebx, 0
    jne .memchk_loop
    jmp .memchk_ok
.memchk_failed:
    mov dword [_mem_chk_result], 0
.memchk_ok:
    
    ; 复位软驱
    xor ah, ah
    xor dl, dl
    int 0x13

    mov ax, base_of_kernel
    mov es, ax
    mov bx, offset_of_kernel
    mov ax, kernel_start_sec_number
    mov cx, kernel_sec_count
.loop:
    cmp cx, 0
    jz LABEL_LOADED
    push cx
    push ax

    mov cl, 1
    call read_sector
    pop ax
    inc ax
    add bx, 512
    pop cx
    dec cx
    jmp .loop

LABEL_LOADED:   
    call kill_motor

;; 准备进入保护模式
    xor eax, eax
    mov ax, ds
    shl eax, 4
    add eax, LABEL_GDT
    mov dword [gdt_ptr+2], eax    
    lgdt [gdt_ptr]
    cli
    in al, 0x92
    or al, 0x02
    out 0x92, al
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; 现在已经是保护模式了，所以跳转应该是选择子加上段内偏移
    jmp dword selector_flat_code:(logic_addr_of_loader+LABEL_PM_START)

kill_motor:
    push dx
    mov dx, 0x03f2
    mov al, 0
    out dx, al
    pop dx
    ret

; 逻辑扇区号是从0开始的，而chs参数扇区号是从1开始的
; ax存放逻辑扇区号，es:bx为存放数据的地址, cl存放扇区的数量
read_sector:
    push bp
    mov bp, sp
    sub esp, 2
    
    mov byte [bp-2], cl              ; 开辟2个字节存放要读取的扇区数量
    push bx
    mov bl, 18
    div bl
    inc ah                           ; 扇区号
    mov cl, ah                       ; 扇区号放在cl中
    mov dh, al
    shr al, 1                        ; 柱面号
    mov ch, al                       ; 柱面号放在ch中
    and dh, 1                        ; 磁头号
    pop bx
    
    mov dl, 0                        ; 驱动器编号
.go_on_reading:
    mov ah, 2                        ; 功能号将磁盘内容读到内存中
    mov al, byte [bp-2]              ; 扇区数量
    int 0x13
    jc .go_on_reading                ; 进位表示出错
    
    add esp, 2
    pop bp
    ret
    
;; es:bp存放字符串的地址
strlen:
    push bx
    mov bx, bp
    mov word [str_len], 0
.loop:    
    mov al, [es:bx]
    cmp al, 0
    jz .done
    inc word [str_len]
    inc bx
    jmp .loop
.done:
    pop bx
    ret

;; es:bp存放字符串
print_msg:
    push cx
    push bx
    push dx
    call strlen
    mov cx, [str_len]
    mov ax, 0x1301
    mov bx, 0x000c
    mov dh, 0
    mov dl, 0
    int 0x10
    pop dx
    pop bx
    pop cx
    ret

;; 实模式下的变量
str_len:        dw 0
loading_msg:    db 'loading .....', 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 下面是保护模式下的代码
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; 保护模式的32位代码
[section .code_32]
align 32
[bits 32]
LABEL_PM_START:
    mov ax, selector_video
    mov gs, ax
    mov ax, selector_flat_rw
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov esp, bottom_of_stack

    push mem_chk_title
    call display_string
    add esp, 4

    call display_mem_size
    call start_paging
    call init_kernel

    jmp selector_flat_code:logic_addr_of_dst_kernel ; 转移到内核中执行代码

;; 显示内存信息
display_mem_size:
    push esi
    push edi
    push ecx
    
    mov esi, mem_chk_buffer
    mov ecx, [mem_chk_result]               ; 读取数据结构的个数
.loop:
    mov edx, 5                              ; 内存信息数据结构有5个字段
    mov edi, ard_struct                     ; 同一结构体反复使用
.1:
    push dword [esi]
    call display_int
    pop eax
    stosd                                   ; stosd dword ptr es:[edi], eax
    add esi, 4
    dec edx
    cmp edx, 0
    jnz .1
    call display_return
    cmp dword [ard_type], 1                 ; 判断是否能被OS使用 cmp dword ptr ds:[], 1
    jne .2                                   ; 直接continue，不参与计算
    mov eax, [base_addr_low]
    add eax, [length_low]
    cmp eax, [mem_size]

    jb .2
    mov [mem_size], eax
    
.2:
    loop .loop
    
    call display_return
    
    push ram_size_prefix
    call display_string
    add esp, 4
    
    push dword [mem_size]
    call display_int
    add esp, 4
    
    pop ecx
    pop edi
    pop esi
    ret

start_paging:    
    ; 根据内存大小计算需要初始化的页目录数目和页表数目
    xor edx, edx
    mov eax, [mem_size]
    mov ebx, 0x400000       ; 一个页表能表示4M的内存
    div ebx
    mov ecx, eax            ; eax为商
    test edx, edx           ; 判断是否有余数
    jz .no_remainder
    inc ecx
.no_remainder:
    push ecx                ; 暂存页表个数到栈
    
    ; 所有现行地址对应物理地址，并不考虑内存空洞
    ; 首先初始化页目录
    mov ax, selector_flat_rw
    mov es, ax
    mov edi, page_dir_base
    xor eax, eax
    mov eax, page_tab_base | PG_P | PG_USU | PG_RWW ; 注意页表开始的地方是写死的
.1:
    stosd
    add eax, 4096   ; 下一个页表, 所有的页表都是连续的
    loop .1
    
    ; 再初始化所有页表
    pop eax                         ; 从栈中弹出页表个数
    mov ebx, 1024                   ; 一个页表占用一页的空间，含有1024个页信息
    mul ebx                         ; 得到页的个数
    mov ecx, eax
    mov edi, page_tab_base          ; 这个基址不像以前是由专门的段规划的
                                     ; 可以直接使用0基址，此时如果使用0会直接崩溃
    xor eax, eax
    mov eax, PG_P | PG_USU | PG_RWW
.2:
    stosd
    add eax, 4096                   ; 每个页4K
    loop .2
    
    mov eax, page_dir_base
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax                    ; 开启分页
    jmp short .3
.3:
    nop
    ret                             ; 开启分页结束

;; 只处理了kernel的program header table
init_kernel:
    xor esi, esi
    mov cx, word [logic_addr_of_src_kernel+0x2c]  ; Elf32_Ehdr->e_phnum，program header个数
                                                    ; 如果磁盘上没有kernel，这个值虚拟机和bochs不一样
    ; 检查合法性
    cmp cx, 0
    jz .no_kernel
    cmp cx, 0xff
    jg .no_kernel
    
    movzx ecx, cx
    mov esi, [logic_addr_of_src_kernel+0x1c] ; elf32_ehdr->e_phoff，program header table的偏移值
    add esi, logic_addr_of_src_kernel        ; 得到了真正的地址
.begin:
    mov eax, [esi+0]            ; header type 为PT_NULL就什么也不做，到下个表项
    cmp eax, 0
    jz .no_action
    push dword [esi+0x10]       ; size header 代表的段的大小，入栈
    mov eax, [esi+0x04]         ; 在文件中的偏移
    add eax, logic_addr_of_src_kernel ;真实地址
    push eax                          ; 原地址入栈
    push dword [esi+0x08]             ; 目的地址入栈
    call memcpy
    add esp, 12
.no_action:
    add esi, 0x20
    dec ecx
    jnz .begin
    ret
    
.no_kernel:
    call display_return    
    push no_kernel_msg
    call display_string
    add esp, 4
    jmp $                           ; 死机

display_al:
    push ecx
    push edx
    push edi
    
    mov	edi, [display_position]

    mov	ah, 0x0f                   ; 0000b: 黑底 1111b: 白字
    mov dl, al
    shr	al, 4
    mov ecx, 2
.begin:
    and	al, 0x0f
    cmp	al, 9
    ja	.1
    add	al, '0'
    jmp	.2
.1:
    sub	al, 0x0a
    add	al, 'A'
.2:
    mov	[gs:edi], ax
    add	edi, 2

    mov al, dl
    loop .begin
    
    mov [display_position], edi

    pop edi
    pop edx
    pop ecx
    ret

display_int:
    mov eax, [esp+4]
    shr eax, 24
    call display_al
    
    mov eax, [esp+4]
    shr eax, 16
    call display_al
    
    mov eax, [esp+4]
    shr eax, 8
    call display_al
    
    mov eax, [esp+4]
    call display_al
    
    mov ah, 0x07                    ; 0000b：黑底 0111b：灰字
    mov al, 'h'
    push edi
    mov edi, [display_position]
    mov [gs:edi], ax
    add edi, 4
    mov [display_position], edi
    pop edi
    ret
    
display_string:
    push ebp
    mov ebp, esp
    push ebx
    push esi
    push edi
    
    mov esi, [ebp+8]
    mov edi, [display_position]
    mov ah, 0x0f
    
.1:
    lodsb
    test al, al
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
    pop edi
    pop esi
    pop ebx
    pop ebp
    ret
    
display_return:
    push return_string
    call display_string
    add esp, 4
    ret

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
    
[section .data]
align 32
[bits 32]
LABEL_DATA:
_pm_msg: db "in protect mode now ...", 0x0a, 0x0a, 0
_mem_chk_title: db "base_low  base_hig  len_low   len_hig   type", 0x0a, 0
_ram_size_prifix: db "ram size: ", 0
_no_kernel_msg: db "no kernel!", 0
_return_string: db 0x0a, 0

_sp_in_real_mode: dw 0
_mem_chk_result: dd 0
_display_position: dd (80*2+0)*2            ; 显示信息的位置
_mem_size: dd 0
_ard_struct:
    _base_addr_low: dd 0
    _base_addr_hig: dd 0
    _length_low: dd 0
    _length_hig: dd 0
    _ard_type: dd 0
ard_struct_size equ $-_ard_struct
    
_mem_chk_buffer: times 256 db 0

pm_msg equ _pm_msg-$$
mem_chk_title equ logic_addr_of_loader+_mem_chk_title
ram_size_prefix equ logic_addr_of_loader+_ram_size_prifix
no_kernel_msg equ logic_addr_of_loader+_no_kernel_msg
return_string equ logic_addr_of_loader+_return_string
display_position equ logic_addr_of_loader+_display_position
mem_size equ logic_addr_of_loader+_mem_size
mem_chk_result equ logic_addr_of_loader+_mem_chk_result
ard_struct equ logic_addr_of_loader+_ard_struct
    base_addr_low equ logic_addr_of_loader+_base_addr_low
    base_addr_hig equ logic_addr_of_loader+_base_addr_hig
    length_low equ logic_addr_of_loader+_length_low
    length_hig equ logic_addr_of_loader+_length_hig
    ard_type equ logic_addr_of_loader+_ard_type
mem_chk_buffer equ logic_addr_of_loader+_mem_chk_buffer

pm_stack:   times 0x1000 db 0
bottom_of_stack equ logic_addr_of_loader+$
