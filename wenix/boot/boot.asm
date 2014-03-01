org 0x7c00
jmp LABEL_START

; 书中的引导扇区过于复杂，而且使用的文件系统是fat12，现在似乎除了dos没有操作系统能够支持
; 以后内核成型了，将这一部分使用grub或者uboot代替，稳定并且可以减少工作量。
%include "loader.inc"

boot_msg:   db 'booting ......', 0
str_len:    dw 0

LABEL_START:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    
    ; 复位软驱
    xor ah, ah
    xor dl, dl
    int 0x13

    mov ax, base_of_loader
    mov es, ax
    mov bx, offset_of_loader
    mov ax, 1
    mov cx, loader_sec_count
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
    mov bp, boot_msg
    mov ax, cs
    mov es, ax
    call print_msg

    jmp base_of_loader:offset_of_loader     ; 进入loader
    
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
    
; 由逻辑扇区号得到chs参数
; 数据在磁盘上存放顺序如下：
; 0面 0磁道 1扇区
; 0面 0磁道 2扇区
; ...
; 0面 0磁道 18扇区
; 1面 0磁道 1扇区
; 1面 0磁道 2扇区
; ...
; 1面 0磁道 18扇区
; 0面 1磁道 1扇区
; 0面 1磁道 2扇区
; ...
; 0面 1磁道 18扇区
; 1面 1磁道 1扇区
; 1面 1磁道 2扇区
; ...
; 1面 1磁道 18扇区
; ...
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
    
times 510-($-$$) db 0
dw 0xaa55