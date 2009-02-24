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
#include <string.h>
#include <vector>
#include <dirent.h>
#include <dlfcn.h>
#include <unistd.h>
#include "creature.h"
#include "pointers.h"
#include "eventmanager.h"
#include "world.h"
#include "object.h"

int main(int argc, const char** argv)
{
  std::vector<int (*)(int, const char**, int&)> argHandlers;
  std::vector<creature*> people;
  eventManager* eventmanager=new eventManager();
  world worldinfo;
  worldinfo.width=500;
  worldinfo.height=500;
  pointers::getInstance()->addPointer(eventmanager, "eventManager");
  pointers::getInstance()->addPointer(&argHandlers, "argHandlers");
  pointers::getInstance()->addPointer(&people, "creatures");
  pointers::getInstance()->addPointer(&worldinfo, "world");
  pointers::getInstance()->addPointer(&Object::iter, "objects");

  DIR* pluginfiles=opendir("plugins"); ///< @todo Make it relative to the executable's location instead of current location
  if(pluginfiles)
  {
    struct dirent* file;
    while((file=readdir(pluginfiles)))
    {
      if(!strcmp(".so", file->d_name+strlen(file->d_name)-3)) ///< @todo Cross platform library filename extensions
      {
        char filename[strlen(file->d_name)+9];
        sprintf(filename, "plugins/%s", file->d_name);
        #ifdef debug
        printf("Loading plugin: %s\n", filename);
        #endif
        void* handle=dlopen(filename, RTLD_LAZY);
        if(!handle){fprintf(stderr, "Failed to load plugin '%s': %s\n", file->d_name, dlerror()); continue;}
        void (*initplugin)(pointers*)=(void (*)(pointers*))dlsym(handle, "initplugin");
        if(const char* error=dlerror()){fprintf(stderr, "Failed to initialize plugin '%s': %s\n", file->d_name, error); dlclose(handle); continue;}
        initplugin(pointers::getInstance());
      }
    }
  }

  // Handle arguments
  for(int i=1; i<argc; i++)
  {
    bool handled=false;
    if(!strcmp(argv[i], "-help")) // Initial help output.
    {
      puts("Available options:");
      puts("-help                             Print this list of options");
      handled=true;
    }

    // Loop through argument handlers provided by plugins.
    for(unsigned int p=0; p<argHandlers.size(); p++)
    {
      if(argHandlers[p](argc, argv, i)!=-1){handled=true; break;}
    }

    if(!strcmp(argv[i], "-help")) return 0;
    if(!handled)
    {
      fprintf(stderr, "Unknown argument '%s', type '%s -help' to see available arguments.\n", argv[i], argv[0]);
      return 1;
    }
  }

  float x, y;
  while(true)
  {
    pointers::getInstance()->getPointerLockWait("creatures");
    for(unsigned int i=0; i<people.size(); i++)
    {
      people[i]->execute();
      if(people[i]->getLife()<=0){
        people[i]->die(); // Isn't life being 0 enough?
        people.erase(people.begin()+i);
        i--;
      }
      people[i]->getPosition(x, y);
      if(x<0)
      {
        x+=worldinfo.width;
        people[i]->setPosition(x, y);
        event warpevent;
        warpevent.name="warp left";
        warpevent.integers.push_back(i);
        pointers::getInstance()->unlockPointer("creatures");
        eventmanager->triggerEvent(warpevent);
        pointers::getInstance()->getPointerLockWait("creatures");
      }
      if(y<0)
      {
        y+=worldinfo.height;
        people[i]->setPosition(x, y);
        event warpevent;
        warpevent.name="warp top";
        warpevent.integers.push_back(i);
        pointers::getInstance()->unlockPointer("creatures");
        eventmanager->triggerEvent(warpevent);
        pointers::getInstance()->getPointerLockWait("creatures");
      }
      if(x>worldinfo.width)
      {
        x-=worldinfo.width;
        people[i]->setPosition(x, y);
        event warpevent;
        warpevent.name="warp right";
        warpevent.integers.push_back(i);
        pointers::getInstance()->unlockPointer("creatures");
        eventmanager->triggerEvent(warpevent);
        pointers::getInstance()->getPointerLockWait("creatures");
      }
      if(y>worldinfo.height)
      {
        y-=worldinfo.height;
        people[i]->setPosition(x, y);
        event warpevent;
        warpevent.name="warp bottom";
        warpevent.integers.push_back(i);
        pointers::getInstance()->unlockPointer("creatures");
        eventmanager->triggerEvent(warpevent);
        pointers::getInstance()->getPointerLockWait("creatures");
      }
    }
    pointers::getInstance()->unlockPointer("creatures");
    usleep(50000);
  }
  return 0;
}
