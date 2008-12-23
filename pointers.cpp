#include <string.h>
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

void pointers::unlockPointer(const char* name)
{
  for(unsigned int i=0; i<pointerlist.size(); i++)
  {
    if(!strcmp(name, pointerlist[i].name)) pointerlist[i].locked=false;
  }
}
