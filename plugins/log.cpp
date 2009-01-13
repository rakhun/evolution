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
#include "../eventmanager.h"
#include "../pointers.h"

FILE* logfile;

bool writelog(event eventobj)
{
  if(!logfile||eventobj.strings.size()<1) return false;
  fputs(eventobj.strings[0], logfile);
  return true;
}

extern "C" {
  void initplugin(pointers* pointersobj)
  {
    if(!(logfile=fopen("evolution.log", "w"))) return;
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("log", writelog);
  }

  void endplugin()
  {
    if(logfile) fclose(logfile);
  }
}
