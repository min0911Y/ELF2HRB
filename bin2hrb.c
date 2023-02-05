#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SaveSegSiz 0x2000
typedef struct {
  uint32_t segSize;  // 数据段的大小
  uint8_t header[4];
  uint32_t res;
  uint32_t stackSize;  // 兼ESP
  uint32_t DataSize;
  uint32_t dataoff;
  char res1[3];
} __attribute__((packed)) hrb_header;

void usage() {
  fprintf(stderr,
          "Usage: bin2hrb <CodeFile> <DataFile> <OutFile> <stackSize> "
          "<EntryPoint>\n");
  exit(1);
}
void bin2hrb(uint8_t* code,
             uint8_t* data,
             uint8_t* out,
             uint32_t stacksz,
             uint32_t entryPoint,
             uint32_t codesz,
             uint32_t datasz) {
  hrb_header hh;
  hh.segSize = stacksz + datasz + SaveSegSiz;
  memcpy(hh.header, "Hari", 4);
  memset(hh.res1, 0, 3);
  hh.res = 0;
  hh.stackSize = stacksz;
  hh.DataSize = datasz;
  hh.dataoff = codesz + sizeof(hrb_header);
  if(entryPoint != 0) {
    hh.dataoff += 5;
  }
  memcpy(out, &hh, sizeof(hrb_header));
  out += sizeof(hrb_header);
  if(entryPoint != 0) {
    out[0] = 0xe9;
    out[1] = (unsigned char)((entryPoint - 0) & 0xff);
    out[2] = (unsigned char)((entryPoint - 0) >> 8);
    out[3] = (unsigned char)((entryPoint - 0) >> 16);
    out[4] = (unsigned char)((entryPoint - 0) >> 24);
    out += 5;
  }
  memcpy(out, code, codesz);
  out += codesz;
  memcpy(out, data, datasz);
}
int main(int argc, char** argv) {
  if (argc != 6) {
    usage();
  }
  FILE *fp_code, *fp_data, *OutFile;
  fp_code = fopen(argv[1], "rb");
  fp_data = fopen(argv[2], "rb");
  OutFile = fopen(argv[3], "wb");
  if (fp_code == -1 || fp_data == -1) {
    printf("No such file.\n");
    exit(1);
  }
  uint32_t csz;
  uint32_t dsz;
  uint32_t stacksz, ep;
  stacksz = strtol(argv[4], NULL, 10);
  ep = strtol(argv[5], NULL, 10);
  fseek(fp_code, 0, SEEK_END);
  csz = ftell(fp_code);
  fseek(fp_code, 0, SEEK_SET);
  fseek(fp_data, 0, SEEK_END);
  dsz = ftell(fp_data);
  fseek(fp_data, 0, SEEK_SET);
  uint32_t OutSize = sizeof(hrb_header) + csz + dsz + 5;
  if (ep == 0) {
    OutSize -= 5;
  }
  uint8_t* code = (uint8_t*)malloc(csz);
  uint8_t* data = (uint8_t*)malloc(dsz);
  uint8_t* out = (uint8_t*)malloc(OutSize);
  fread(code, csz, 1, fp_code);
  fread(data, dsz, 1, fp_data);

  bin2hrb(code, data, out, stacksz, ep, csz, dsz);
  fwrite(out, OutSize, 1, OutFile);
  fclose(fp_code);
  fclose(fp_data);
  fclose(OutFile);
  return 0;
}