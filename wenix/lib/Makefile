.PHONY: all

all: klib.asm string.asm
	nasm -f elf -o string.o string.asm
	nasm -f elf -o klib_a.o klib_a.asm

.PHONY: clean

clean:
	rm -f *.o