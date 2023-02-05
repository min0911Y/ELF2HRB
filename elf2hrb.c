#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdarg.h>
#define DELFILE_CMD "del "
/* Type for a 16-bit quantity.  */
typedef uint16_t Elf32_Half;

/* Types for signed and unsigned 32-bit quantities.  */
typedef uint32_t Elf32_Word;

/* Type of addresses.  */
typedef uint32_t Elf32_Addr;

/* Type of file offsets.  */
typedef uint32_t Elf32_Off;

typedef struct
{
  unsigned char e_ident[16]; /* Magic number and other info */
  Elf32_Half    e_type;         /* Object file type */
  Elf32_Half    e_machine;      /* Architecture */
  Elf32_Word    e_version;      /* Object file version */
  Elf32_Addr    e_entry;        /* Entry point virtual address */
  Elf32_Off e_phoff;        /* Program header table file offset */
  Elf32_Off e_shoff;        /* Section header table file offset */
  Elf32_Word    e_flags;        /* Processor-specific flags */
  Elf32_Half    e_ehsize;       /* ELF header size in bytes */
  Elf32_Half    e_phentsize;        /* Program header table entry size */
  Elf32_Half    e_phnum;        /* Program header table entry count */
  Elf32_Half    e_shentsize;        /* Section header table entry size */
  Elf32_Half    e_shnum;        /* Section header table entry count */
  Elf32_Half    e_shstrndx;     /* Section header string table index */
} elf32;
void usage() {
	fprintf(stderr, "Usage: elf2hrb <elfFile> <hrbFile> <stackSize>\n");
	exit(1);
}
int systemFormat(char *fmt,...) {
  int len;
  va_list ap;
  va_start(ap, fmt);
  char buf[1024];
  vsprintf(buf, fmt, ap);
  printf("Sys %s\n",buf);
  len = system(buf);
  va_end(ap);
  return len;
}
int main(int argc, char const *argv[])
{
	if (argc != 4) {
		usage();
	}
	uint32_t entry;
	FILE *fp = fopen(argv[1],"rb");
	fseek(fp,0,SEEK_END);
	int sz = ftell(fp);
	fseek(fp,0,SEEK_SET);
	uint8_t *buf = malloc(sz);
	fread(buf,sz,1,fp);
	elf32 *e = buf;
	systemFormat("objcopy -O binary -R .data -R .rodata -R .bss %s code_%s.bin",argv[1],argv[1]);
	systemFormat("objcopy -O binary -R .text %s data_%s.bin",argv[1],argv[1]);
	systemFormat("bin2hrb code_%s.bin data_%s.bin %s %d %d",argv[1],argv[1],argv[2],strtol(argv[3],NULL,10),e->e_entry);
	systemFormat(DELFILE_CMD"code_%s.bin data_%s.bin",argv[1],argv[1]);
	return 0;
}