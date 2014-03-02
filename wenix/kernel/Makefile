ASM := nasm
ASM_OPTION := -f elf
ASM_OUTPUT_OPTION := -o

CC := gcc
CC_OPTION := -c -Wall -fno-builtin
CFLAGS := -I../include
CC_OUTPUT_OPTION := -o

obj = global.o i8259a.o intrrupt.o irq.o irq_imp.o kernel.o printk.o start.o

all: $(obj)
	ar -r kernel.a $^

%.o: %.s
	$(ASM) $(ASM_OPTION) $< $(ASM_OUTPUT_OPTION) $@

%.o: %.c
	$(CC) $(CFLAGS) $(CC_OPTION) $< $(CC_OUTPUT_OPTION) $@

.PHONY: all clean

clean:
	rm -f *.o
	rm -f *.a
