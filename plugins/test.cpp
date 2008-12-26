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

bool eventtest(event eventobj)
{
  printf("Received test event, it contained the integer %i\n", eventobj.integers[0]);
}

extern "C" {
  void initplugin(pointers* pointersobj)
  {
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("Test", eventtest);
  }
}
