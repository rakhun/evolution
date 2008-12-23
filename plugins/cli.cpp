#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../pointers.h"
#include "../creature.h"

void* commandinput(void* pointer)
{
  char buffer[1024];
  sleep(1); // Wait for initial output to finish
  while(true)
  {
    printf("Evolution> ");
    fflush(stdout);
    if(!fgets(buffer, 1024, stdin)) continue;
    int tmp=strlen(buffer)-1;
    if(buffer[tmp]=='\n') buffer[tmp]=0;
    int argc=1;
    for(unsigned int i=0; i<strlen(buffer); i++)
    {
      if(buffer[i]==' ') argc++;
    }
    char** argv=new char*[argc];
    char* arg=buffer;
    unsigned int i=0;
    while(i<argc)
    {
      char* tmp=strchrnul(arg, ' ');
      tmp[0]=0;
      argv[i]=new char[tmp-arg+1];
      strcpy(argv[i], arg);
      if(tmp[0]=' ') arg=tmp+1;
      i++;
    }
    /// @todo Delete this mem when you're done
    if(argv[0]&&argv[0][0])
    {
      if(!strcmp(argv[0], "exit"))
      {
        exit(0);
      }
      else if(!strcmp(argv[0], "help"))
      {
        puts("Available commands:");
        puts("exit                  Exits EvolutionBots");
        puts("create [type] [X] [Y] Creates a new creature at X, Y.");
        puts("                       Type=default or manual (using a default");
        puts("                       COL script or entering code manually)");
      }
      else if(!strcmp(argv[0], "create"))
      {
        if(argc!=4)
        {
          puts("Usage: create [type=default/manual] [X] [Y]");
          continue;
        }
        if(!strcmp("manual", argv[1]))
        {
          puts("Manual creation not yet implemented.");
        }else{ // Default to 'default'
          /// @todo Write some proper default code that actually does something.
          creature* baby=new creature(atoi(argv[2]), atoi(argv[3]), (const unsigned char*)"");
          std::vector<creature*>* people=(std::vector<creature*>*)((pointers*)pointer)->getPointerLockWait("creatures");
          people->push_back(baby);
          printf("Number of creatures in the people vector: %i\n", people->size());
          ((pointers*)pointer)->unlockPointer("creatures");
        }
      }
      else{printf("Unknown command '%s'. Type 'help' to get a list of available commands\n", argv[0]);}
    }
    for(unsigned int i=0; i<argc; i++) delete[] argv[i];
    delete[] argv;
  }
}

extern "C" void initplugin(pointers* pointerobj)
{
  pthread_t thread;
  pthread_create(&thread, NULL, commandinput, pointerobj);
}
