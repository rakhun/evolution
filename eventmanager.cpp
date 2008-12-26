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
