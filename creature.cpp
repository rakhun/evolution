/// @file creature.cpp
/// This file holds most of the creature class' functionality, except for COL execution which resides in col.cpp
/// @brief Main code for the creature class
#include <vector>
#include <string.h>
#include "creature.h"

creature::creature(int x, int y, const unsigned char* code)
{
  this->x=x;
  this->y=y;
  const unsigned char* pos=code;
  std::vector<unsigned char> commands;
  std::vector<unsigned char> args;
  col_length=0;
  pointer=0;
  health=100;
  memset(mem, 0, 512);
  mempointer=0;
  angle=0;
  while(pos!=(void*)1)
  {
    bool validcommand=false;
    unsigned char command=0;
    unsigned char arg=0;
    unsigned char C=0;
    while(pos[C]!=':'&&pos[C]!=0&&pos[C]!='\n')
    {
      if(charToNumber(pos[C])!=255)
      {
        command*=16;
        command+=charToNumber(pos[C]);
        validcommand=true;
      }
      C++;
    }
    while(pos[C]!='\n'&&pos[C]!=0)
    {
      if(charToNumber(pos[C])!=255)
      {
        arg*=16;
        arg+=charToNumber(pos[C]);
        validcommand=true;
      }
      C++;
    }
    if(validcommand)
    {
      col_length++;
      commands.push_back(command);
      args.push_back(arg);
    }
    pos=(const unsigned char*)strchr((const char*)pos+1, '\n')+1;
  }
  col=new unsigned char[col_length*2];
  for(unsigned int C=0; C<col_length; C++)
  {
    col[C*2]=commands[C];
    col[C*2+1]=args[C];
  }
  commands.clear();
  args.clear();
}

void creature::getCOL(unsigned char*& col, unsigned int& col_length)
{
  col=this->col;
  col_length=this->col_length;
}

void creature::getPosition(int& x, int& y)
{
  x=this->x;
  y=this->y;
}

void creature::setPosition(int x, int y)
{
  this->x=x;
  this->y=y;
}
