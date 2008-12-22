#include <stdio.h>
#include "../eventmanager.h"
#include "../pointers.h"

bool eventtest(event eventobj)
{
  printf("Received test event, it contained the integer %i\n", eventobj.integers[0]);
}

extern "C" {
  void initplugin(pointers* pointersobj)
  {
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("Test", eventtest);
  }
}
