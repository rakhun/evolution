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
#include <stdlib.h>
#include "../eventmanager.h"
#include "../pointers.h"

FILE* logfile;

bool writelog(event eventobj)
{
  if(eventobj.strings.size()<1) return false;
  if(!logfile&&!(logfile=fopen("evolution.log", "w"))) return false;
  fputs(eventobj.strings[0], logfile);
  return true;
}

int handleArg(int argc, const char** argv, int& i)
{
  if(!strcmp(argv[i], "-logfile")){
    if(i+1>=argc)
    {
      fputs("-logfile: missing argument\n", stderr);
      exit(1);
    }
    i++;
    if(logfile) fclose(logfile);
    logfile=fopen(argv[i], "w");
  }else if(!strcmp(argv[i], "-help")){
    puts("-logfile [file]                   Write log messages to [file]");
    return -1;
  }
  return 0;
}

extern "C" {
  void initplugin(pointers* pointersobj)
  {
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("log", writelog);
    ((std::vector<int (*)(int, const char**, int&)>*)pointersobj->getPointer("argHandlers"))->push_back(handleArg);
  }

  __attribute__((destructor))
  void endplugin()
  {
    if(logfile) fclose(logfile);
  }
}
