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
#include <string.h>
#include <unistd.h>
#include "pointers.h"

pointers* pointers::instance;

pointers* pointers::getInstance()
{
  if(!instance){instance=new pointers();}
  return instance;
}

void pointers::addPointer(void* pointer, const char* name)
{
  pointerInfo newpointer;
  newpointer.pointer=pointer;
  newpointer.name=name;
  newpointer.locked=false;
  pointerlist.push_back(newpointer);
}

void* pointers::getPointer(const char* name)
{
  for(unsigned int i=0; i<pointerlist.size(); i++)
  {
    if(!pointerlist[i].locked&&!strcmp(name, pointerlist[i].name)) return pointerlist[i].pointer;
  }
  return 0;
}

void* pointers::getPointerLock(const char* name)
{
  for(unsigned int i=0; i<pointerlist.size(); i++)
  {
    if(!pointerlist[i].locked&&!strcmp(name, pointerlist[i].name))
    {
      pointerlist[i].locked=true;
      return pointerlist[i].pointer;
    }
  }
  return 0;
}

void* pointers::getPointerLockWait(const char* name)
{
  for(unsigned int i=0; i<pointerlist.size(); i++)
  {
    if(!strcmp(name, pointerlist[i].name))
    {
      while(pointerlist[i].locked) usleep(1);
      pointerlist[i].locked=true;
      return pointerlist[i].pointer;
    }
  }
  return 0;
}

void pointers::unlockPointer(const char* name)
{
  for(unsigned int i=0; i<pointerlist.size(); i++)
  {
    if(!strcmp(name, pointerlist[i].name)) pointerlist[i].locked=false;
  }
}
