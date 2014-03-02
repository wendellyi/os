ASM := nasm
ASM_OPTION := -f elf
ASM_FLAGS := -I../include

CC := gcc
CC_OPTION := -c -Wall -fno-builtin
CFLAGS := -I../include

obj = stdlib.o string.o vsprintf.o system.o console.o 

all: $(obj)
	ar -r lib.a $^

%.o: %.asm
	$(ASM) $(ASM_FLAGS) $(ASM_OPTION) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(CC_OPTION) $< -o $@

system.o:
	$(MAKE) -C asm
	yes | cp asm/system.o .

.PHONY: all clean system.o

clean:
	rm -f *.o *.a
	$(MAKE) clean -C asm