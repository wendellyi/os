all:
	nasm -o boot.bin boot.asm
	nasm -o loader.bin loader.asm
    
.PHONY: clean all

clean:
	rm -f *.bin