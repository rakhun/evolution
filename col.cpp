/// @file col.cpp
/// This file holds the creature class' execute function which interprets the COL (Code Of Life) scripting language
/// @brief Code for interpreting COL (Code Of Life)
#include <stdio.h>
#include "creature.h"

void creature::execute()
{
  unsigned char arg=col[pointer*2+1];
  switch(col[pointer*2])
  {
  case 0:
    break;
  case 1:
    break;
  case 2:
    if(arg>127)
      pointer+=arg-127;
    else
      pointer+=arg-128;
    break;
  case 3:
    break;
  case 4:
    if(arg>127) // Always go back one more step so that +0 (which we don't have) would result in a loop
      mempointer+=arg-128;
    else
      mempointer+=arg-129;
    if(mempointer>511) mempointer-=512;
    if(mempointer<0) mempointer+=512;
    break;
  case 5:
    if(mem[mempointer]!=arg) pointer++;
    break;
  case 6:
    if(mem[mempointer]==arg) pointer++;
    break;
  case 7:
    if(mem[mempointer]<=arg) pointer++;
    break;
  case 8:
    if(mem[mempointer]>=arg) pointer++;
    break;
  case 9:
    break;
  case 10:
    break;
  case 11:
    break;
  default:
    printf("Command not implemented: %i\n", col[pointer]);
  }
  pointer++;
  if(pointer>=col_length) pointer-=col_length;
}
