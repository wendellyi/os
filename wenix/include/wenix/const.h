#ifndef _WENIX_CONST_H_
#define _WENIX_CONST_H_

#define NR_GDT                  6
#define NR_IDT                  256
#define NR_TASK                 64

#define SELECTOR_KERNEL_CS      8

#define FIRST_TSS_ENTRY         4                       /* 初始状态只用了前面4个，所以够用 */
#define FIRST_LDT_ENTRY         (FIRST_TSS_ENTRY+1)

#define DA_DPL0                 0x00
#define DA_DPL3                 0x60

#define DPL_KERNEL              0
#define DPL_USER                3

#define DA_386INTGATE           0x8e

#define	INT_VECTOR_IRQ0			0x20
#define	INT_VECTOR_IRQ8			0x28

#define INT_M_CTL               0x20
#define INT_M_CTL_MASK          0x21
#define INT_S_CTL               0xa0
#define INT_S_CTL_MASK          0xa1



/* 字符的颜色 */
#define CHAR_COLOR_DEFAULT      0x0c

#endif /* _WENIX_CONST_H */
