OUT_PUT := wenix.img
ENTRY_POINT := 0x30400

LD := ld
LD_FLAGS = -m elf_i386 -s -Ttext $(ENTRY_POINT)

all: boot.bin kernel.bin
	dd of=$(OUT_PUT) if=boot.bin bs=512 count=1
	dd of=$(OUT_PUT) if=loader.bin bs=512 seek=1
	dd of=$(OUT_PUT) if=kernel.bin bs=512 seek=32

boot.bin:
	$(MAKE) -C boot
	yes | cp boot/boot.bin .
	yes | cp boot/loader.bin .

kernel.bin: lib.a kernel.a
	$(LD) $(LD_FLAGS) -o kernel.bin kernel.a lib.a 

lib.a:
	$(MAKE) -C lib
	yes | cp lib/lib.a .

kernel.a:
	$(MAKE) -C kernel
	yes | cp kernel/kernel.a .

.PHONY:	all clean boot.bin lib.a kernel.a
	
clean:
	rm -fr *.bin *.o *.a *.img
	$(MAKE) clean -C kernel
	$(MAKE) clean -C lib
	$(MAKE) clean -C boot
