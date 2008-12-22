#include <stdio.h>
#include <string.h>
#include <vector>
#include <dirent.h>
#include <dlfcn.h>
#include "creature.h"
#include "pointers.h"
#include "eventmanager.h"
#include "world.h"

std::vector<void*> plugins;
std::vector<int (*)(int, const char**, int&)> argHandlers;

int main(int argc, const char** argv)
{
  std::vector<creature*> people;
  eventManager* eventmanager=new eventManager();
  world worldinfo;
  worldinfo.width=500;
  worldinfo.height=500;
  pointers::getInstance()->addPointer(eventmanager, "eventManager");
  pointers::getInstance()->addPointer(&argHandlers, "argHandlers");
  pointers::getInstance()->addPointer(&people, "creatures");
  pointers::getInstance()->addPointer(&worldinfo, "world");

  DIR* pluginfiles=opendir("plugins"); ///< @todo Make it relative to the executable's location instead of current location
  struct dirent* file;
  while(file=readdir(pluginfiles))
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
//      plugvoid_vectorint_intcharint initplugin=(plugvoid_vectorint_intcharint)dlsym(handle, "initplugin");
      void (*initplugin)(pointers*)=(void (*)(pointers*))dlsym(handle, "initplugin");
      if(const char* error=dlerror()){fprintf(stderr, "Failed to initialize plugin '%s': %s\n", file->d_name, error); continue;}
//      initplugin(&argHandlers);
      initplugin(pointers::getInstance());
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
    for(int p=0; p<argHandlers.size(); p++)
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

  while(true)
  {
    for(unsigned int i=0; i<people.size(); i++)
    {
      people[i]->execute();
      if(people[i]->getLife()>0){
        i++;
      }else{
        people[i]->die(); // Isn't life being 0 enough?
        people.erase(people.begin()+i);
      }
    }
  }
  return 0;
}
