ASM := nasm
ASM_OPTION := -f elf
ASM_OUTPUT_OPTION := -o

CC := gcc
CC_OPTION := -c -Wall
CFLAGS := -I../include
CC_OUTPUT_OPTION := -o

all: kernel.o start.o i8259a.o irq.o protect.o

%.o: %.asm
	$(ASM) $(ASM_OPTION) $< $(ASM_OUTPUT_OPTION) $@

%.o: %.c
	$(CC) $(CFLAGS) $(CC_OPTION) $< $(CC_OUTPUT_OPTION) $@

.PHONY: all clean

clean:
	rm -f *.o