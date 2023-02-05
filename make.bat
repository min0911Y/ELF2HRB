nasm -f elf32 Sys.asm -o Sys.o
i686-elf-gcc -m32 -nostdinc -nostdlib -ffreestanding -fno-stack-protector -Qn -fno-pic -fno-pie -fno-asynchronous-unwind-tables -mpreferred-stack-boundary=2 -fomit-frame-pointer -Qn -O0 -mno-mmx -mno-sse -mfpmath=387 -c test.c
i686-elf-ld -m elf_i386 -T link.ld Sys.o test.o -o a.elf -e _Start
