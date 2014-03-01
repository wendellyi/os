.PHONY: all

all:
	nasm -o boot.bin boot.asm
	nasm -o loader.bin loader.asm
    
.PHONY: clean

clean:
	rm -f *.bin