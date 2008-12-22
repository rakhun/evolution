#include <string.h>
#include "eventmanager.h"

void eventManager::registerEvent(const char* name, bool (*callback)(event))
{
  eventCallback x;
  x.name=name;
  x.callback=callback;
  callbacks.push_back(x);
}

bool eventManager::triggerEvent(event eventobj)
{
  bool handled=false;
  for(unsigned int i=0; i<callbacks.size(); i++)
  {
    if(!strcmp(eventobj.name, callbacks[i].name))
    {
      if(callbacks[i].callback(eventobj)) handled=true;
    }
  }
  return handled;
}
