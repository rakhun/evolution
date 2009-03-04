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
    int i=0;
    while(i<argc) // Fill in arguments into argv
    {
      char* tmp=strchrnul(arg, ' ');
      tmp[0]=0;
      argv[i]=new char[tmp-arg+1];
      strcpy(argv[i], arg);
      arg=tmp+1;
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
        puts("printcol [ID]         Print the COL of a creature");
        puts("ls                    List creatures and some values");
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
          std::vector<char*> lines;
          int size=0;
          while(true)
          {
            char* r;
            char* line=new char[128];
            r=fgets(line, 128, stdin);
            if(r==0||line[1]==0) break;
            lines.push_back(line);
            size+=strlen(line);
          }
          unsigned char* col=new unsigned char[size+1];
          col[size]=0;
          int pos=0;
          for(unsigned int i=0; i<lines.size(); i++)
          {
            strcpy((char*)(pos+col), lines[i]);
            pos+=strlen(lines[i]);
          }
          creature* baby=new creature(atoi(argv[2]), atoi(argv[3]), col);
          std::vector<creature*>* people=(std::vector<creature*>*)((pointers*)pointer)->getPointerLockWait("creatures");
          people->push_back(baby);
          printf("Number of creatures in the people vector: %i\n", people->size());
          ((pointers*)pointer)->unlockPointer("creatures");
        }else{ // Default to 'default'
          /// @todo Write some proper default code that actually does something.
          creature* baby=new creature(atoi(argv[2]), atoi(argv[3]), (const unsigned char*)"0:FF\n0:FF\n1:80");
          std::vector<creature*>* people=(std::vector<creature*>*)((pointers*)pointer)->getPointerLockWait("creatures");
          people->push_back(baby);
          printf("Number of creatures in the people vector: %i\n", people->size());
          ((pointers*)pointer)->unlockPointer("creatures");
        }
      }
      else if(!strcmp(argv[0], "ls"))
      {
        std::vector<creature*>* people=(std::vector<creature*>*)((pointers*)pointer)->getPointerLockWait("creatures");
        printf("%i creatures:\n", people->size());
        for(unsigned int i=0; i<people->size(); i++)
        {
          float x, y;
          people->at(i)->getPosition(x, y);
          unsigned char* col;
          unsigned int col_length;
          people->at(i)->getCOL(col, col_length);
          printf("ID: %i, X: %f, Y: %f, Health: %f, COL-length: %i\n", i, x, y, people->at(i)->getLife(), col_length);
        }
        ((pointers*)pointer)->unlockPointer("creatures");
      }
      else if(!strcmp(argv[0], "printcol"))
      {
        if(argc!=2)
        {
          puts("Usage: printcol [creature ID]");
          continue;
        }
        std::vector<creature*>* people=(std::vector<creature*>*)((pointers*)pointer)->getPointerLockWait("creatures");
        if(atoi(argv[1])<0||atoi(argv[1])>=people->size())
        {
          puts("ID out of range");
          ((pointers*)pointer)->unlockPointer("creatures");
          continue;
        }
        unsigned char* col;
        unsigned int col_length;
        people->at(atoi(argv[1]))->getCOL(col, col_length);
        for(unsigned int i=0; i<col_length*2; i+=2)
        {
          printf("%x:%x\n", col[i], col[i+1]);
        }
        ((pointers*)pointer)->unlockPointer("creatures");
      }
      else{printf("Unknown command '%s'. Type 'help' to get a list of available commands\n", argv[0]);}
    }
    for(int i=0; i<argc; i++) delete[] argv[i];
    delete[] argv;
  }
}

extern "C" {
  void initplugin(pointers* pointerobj)
  {
    pthread_t thread;
    pthread_create(&thread, NULL, commandinput, pointerobj);
  }

  __attribute__((destructor))
  void endplugin()
  {
    puts(""); // Just print a newline so the shell won't begin on the same line
  }
}
