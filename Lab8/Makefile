C_FLAGS = -g -c 
ASM_FLAGS = -felf64 -g 

.GO:all

all: 
	rm -rf bin
	mkdir bin
	gcc $(C_FLAGS) bmp_struct.c -o bin/bmp_struct.o
	gcc $(C_FLAGS) sepia_c.c -o bin/sepia_c.o
	gcc $(C_FLAGS) sepia_sse.c -o bin/sepia_sse.o
	nasm $(ASM_FLAGS) sepia_asm.asm -o bin/sepia_asm.o
	gcc $(C_FLAGS) main.c -o bin/main.o
	gcc -g bin/*.o -o bin/main
	rm -rf bin/*.o

clean:
	rm -f main

