ASM := nasm
ASM_OPTION := -f elf

all: system.s
	$(ASM) $(ASM_OPTION) system.s -o system.o

.PHONY: clean

clean:
	rm -f *.o