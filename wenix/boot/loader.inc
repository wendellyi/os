base_of_loader equ 0x9000           ; loader.bin被加载的段地址
offset_of_loader equ 0x0100         ; loader.bin被加载的偏移地址
rm_bottom_of_stack equ 0x0100       ; 实模式下的栈底
loader_sec_count equ 16             ; loader占用的扇区数量

logic_addr_of_loader equ base_of_loader*0x10        ; loader.bin被加载的逻辑地址

base_of_kernel equ 0x8000           ; 内核被加载的段地址
offset_of_kernel equ 0x00           ; 内核被加载的偏移地址

logic_addr_of_src_kernel equ base_of_kernel*0x10
logic_addr_of_dst_kernel equ 0x30400 ;; 必须与Makefile中的-Ttext参数值相等!!

kernel_start_sec_number equ 32      ; kernel开始读入的逻辑扇区号
kernel_sec_count equ 16             ; kernel占用的扇区数量

page_dir_base equ 0x200000      ;; 页目录起始之处2M
page_tab_base equ 0x201000      ;; 页表起始之处2M+4K
