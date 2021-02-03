ASM=nasm
ASMFLAGS=-f elf64 -o

lab2: dict.o lib.o main.o
	ld -o lab2 dict.o lib.o main.o

dict.o: dict.asm
	$(ASM) $(ASMFLAGS) dict.o dict.asm

lib.o: lib.asm
	$(ASM) $(ASMFLAGS) lib.o lib.asm

main.o: main.asm colon.inc words.inc
	$(ASM) $(ASMFLAGS) main.o main.asm

clear:
	rm -f main.o lib.o dict.o lab2

