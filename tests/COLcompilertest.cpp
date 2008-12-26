/*
 * Evolution - An experiment with evolving scripts through natural selection
 * Copyright (C) 2008 Henrik Nilsson
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not, see <http://www.gnu.org/licenses>.
 */
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
