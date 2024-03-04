#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../structs/dataflash.h"

inline static uint16_t byteswap(uint16_t in)
{
    return (in >> 8) | (in << 8);
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  FILE* fp = fopen("abbydf.bin","rb+");

  fseek(fp, 0x8B, SEEK_SET);
  subclass_0x52 s52;
  fread(&s52, sizeof(s52),1, fp);

  printf("cycle_count: 0x%04X\n", byteswap(s52.cycle_count));

  fseek(fp, 0xF9, SEEK_SET);
  subclass_0x53 s53;
  fread(&s53, sizeof(s53),1, fp);

  printf("chem_id: 0x%04X, qmax: 0x%04X\n", byteswap(s53.chem_id), byteswap(s53.qmax_cell_0));


  fseek(fp, 0x271, SEEK_SET);
  subclass_0x30 s30;
  fread(&s30, sizeof(s30),1, fp);

  printf("design_capacity: 0x%04X\n", byteswap(s30.design_capacity));

  printf("Input new capacity(mAh): ");
  char capacity[256];
  fgets(capacity, 255, stdin);
  s30.design_capacity = byteswap(atoi(capacity));
  s53.qmax_cell_0 = byteswap(atoi(capacity) / 100 * 103);
  s52.cycle_count = 0;


  printf("New capacity: 0x%04X\n", byteswap(s30.design_capacity));
  printf("New qmax: 0x%04X\n", byteswap(s53.qmax_cell_0));
  printf("New cycle_count: 0\n");

  fseek(fp, 0x271, SEEK_SET);
  fwrite(&s30, sizeof(s30),1, fp);

  fseek(fp, 0xF9, SEEK_SET);
  fwrite(&s53, sizeof(s53),1, fp);

  fseek(fp, 0x8B, SEEK_SET);
  fwrite(&s52, sizeof(s52),1, fp);

  fclose(fp);

  return 0;
}
