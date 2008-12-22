#include <stdio.h>
#include <string.h>
#include "../pointers.h"
#include "../eventmanager.h"

bool transferCreature(event eventobj)
{
  printf("network.so: Received event to transfer a creature on the %s, but networking is so far just a stub\n", eventobj.name+5);
}

int handleArg(int argc, const char** argv, int &i)
{
  if(!strcmp(argv[i], "-connect")){
    if(i+1>=argc)
    {
      fputs("-connect: missing argument\n", stderr);
      return 0;
    }
    i++;
    fputs("-connect: not yet implemented\n", stderr);
    return 0;
  }else if(!strcmp(argv[i], "-listen")){
    fputs("-listen: not yet implemented\n", stderr);
    return 0;
  }else if(!strcmp(argv[i], "-help")){
    puts("-connect [server to connect to]   Connect to another virtual world to extend it geographically");
    puts("-listen                           Listen for incoming connections to extend the virtual world");
  }
  return -1; // = not handled/continue anyway
}

extern "C" {
  void initplugin(pointers* pointersobj)
  {
    ((std::vector<int (*)(int, const char**, int&)>*)pointersobj->getPointer("argHandlers"))->push_back(handleArg);
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("warp top", transferCreature);
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("warp bottom", transferCreature);
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("warp left", transferCreature);
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("warp right", transferCreature);
  }
}
