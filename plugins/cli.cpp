#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../pointers.h"

void* commandinput(void* pointer)
{
  char buffer[1024];
  sleep(1); // Wait for initial output to finish
  while(true)
  {
    printf("Evolution> ");
    fflush(stdout);
    fgets(buffer, 1024, stdin);
    int tmp=strlen(buffer)-1;
    if(buffer[tmp]=='\n') buffer[tmp]=0;
    //if(!strncmp(buffer, "command ", 8))
    if(!strcmp(buffer, "exit"))
    {
      exit(0);
    }
    else if(!strcmp(buffer, "help"))
    {
      puts("Available commands:");
      puts("exit         Exits EvolutionBots");
    }
    else{puts("Unknown command. Type 'help' to get a list of available commands");}
  }
}

extern "C" void initplugin(pointers* pointerobj)
{
  pthread_t thread;
  pthread_create(&thread, NULL, commandinput, pointerobj);
}
