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
/// @file creature.cpp
/// This file holds most of the creature class' functionality, except for COL execution which resides in col.cpp
/// @brief Main code for the creature class
#include <vector>
#include <string.h>
#include "creature.h"
#include "object.h"
#include "pointers.h"

creature::creature(int x, int y, const unsigned char* code)
{
  Object** tmpobj=(Object**)pointers::getInstance()->getPointerLockWait("objects");
  if(!tmpobj)
  {
    /* Log this error */
  }else{
    this->next=*tmpobj;
    *tmpobj=this;
  }
  pointers::getInstance()->unlockPointer("objects");
  this->x=x;
  this->y=y;
  this->objType=1;
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

creature::creature(float x, float y, float angle, unsigned char* col, unsigned int col_length, unsigned int pointer, unsigned char* mem, int mempointer, int health)
{
  this->x=x;
  this->y=y;
  this->angle=angle;
  this->col=col; // Bad, redo this, allocating new memory and memcpy:ing
  this->col_length=col_length;
  this->pointer=pointer;
  memcpy(this->mem, mem, 512);
  this->mempointer=mempointer;
  this->health=health;
}

void creature::getCOL(unsigned char*& col, unsigned int& col_length)
{
  col=this->col;
  col_length=this->col_length;
}

float creature::getAngle()
{
  return angle;
}

unsigned int creature::getPointer()
{
  return pointer;
}

int creature::getMemPointer()
{
  return mempointer;
}
