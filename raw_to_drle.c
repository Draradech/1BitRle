#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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
  a_perror(argc == 5, "argc");
  
  FILE *fin = fopen(argv[1], "rb");
  FILE *fout = fopen(argv[2], "wb");
  
  a_perror(fin != NULL, "open fin");
  a_perror(fout!= NULL, "open fout");

  uint16_t w = atoi(argv[3]);
  uint16_t h = atoi(argv[4]);
  
  a_perror(w > 0 && w <= 16384 && h > 0 && h <= 16384, "dimensions");

  a_perror(1 == fwrite(&w, 2, 1, fout), "write w");
  a_perror(1 == fwrite(&h, 2, 1, fout), "write h");
  
  uint8_t col = 0;
  uint16_t len = 0;
  
  uint8_t pix;
  
  for (int i = 0; i < w * h; i++)
  {
    a_perror(1 == fread(&pix, 1, 1, fin), "read pix");
    if (pix != col)
    {
      a_perror(1 == fwrite(&len, 2, 1, fout), "write len");
      len = 1;
      col = !col;
    }
    else
    {
      len++;
      if (len == 0xffff)
      {
        a_perror(1 == fwrite(&len, 2, 1, fout), "write len");
        len = 0;
        a_perror(1 == fwrite(&len, 2, 1, fout), "write len");
      }
    }
  }
  a_perror(1 == fwrite(&len, 2, 1, fout), "write len");
  a_perror(0 == fclose(fin), "close fin");
  a_perror(0 == fclose(fout), "close fout");
}
