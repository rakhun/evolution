#include <stdio.h>
#include "../creature.h"

int main(){
  creature test(0, 0, (const unsigned char*)"F0:0D\n2:7f\n");
  unsigned char* col;
  unsigned int length;
  test.getCOL(col, length);
  printf("The COL shortcode compiler generated a %u byte long code.\n", length*2);
  printf("The following commands were used:\n");
  for(unsigned int i=0; i<length*2; i+=2)
  {
    printf("%X:%X\n", col[i], col[i+1]);
  }
}
