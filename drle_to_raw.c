#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void a_perror(int val, char *msg)
{
  if (!val)
  {
    perror(msg);
    exit(-1);
  }
}

int main(int argc, char *argv[])
{
  a_perror(argc == 3, "argc");
  
  FILE *fin = fopen(argv[1], "rb");
  FILE *fout = fopen(argv[2], "wb");
  
  a_perror(fin != NULL, "open fin");
  a_perror(fout!= NULL, "open fout");

  uint16_t w;
  uint16_t h;
  a_perror(1 == fread(&w, 2, 1, fin), "read w");
  a_perror(1 == fread(&h, 2, 1, fin), "read h");
  
  a_perror(w > 0 && w <= 16384 && h > 0 && h <= 16384, "dimensions");
  
  uint8_t col = 0;
  uint16_t len = 0;
  uint8_t *imgdata = malloc(w * h);
  uint8_t *seek = imgdata;
  
  a_perror(imgdata != NULL, "malloc");
  
  int ok = fread(&len, 2, 1, fin);
  while (ok)
  {
    memset(seek, col, len);
    seek += len;
    col = !col;
    ok = fread(&len, 2, 1, fin);
  }
  
  a_perror(w * h == fwrite(imgdata, 1, w * h, fout), "write imgdata");
  
  a_perror(0 == fclose(fin), "close fin");
  a_perror(0 == fclose(fout), "close fout");
}
