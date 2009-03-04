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
/// @file col.cpp
/// This file holds the creature class' execute function which interprets the COL (Code Of Life) scripting language
/// @brief Code for interpreting COL (Code Of Life)
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "creature.h"
#include "pointers.h"
#include "eventmanager.h"
#include "log.h"

void creature::execute()
{
  health-=0.0001;
  if(!col_length) return; // Creatures like that ought to die.. but nature will deal with that
  unsigned char arg=col[pointer*2+1];
  event eventobj;
  eventManager* eventMgr;
  switch(col[pointer*2])
  {
  case 0: // Walk
    if(arg>127)
    {
      x+=cos(angle)*(arg-127)/64;
      y+=sin(angle)*(arg-127)/64;
      health-=fabsf(arg-127)/10240;
    }else{
      x+=cos(angle)*(arg-128)/64;
      y+=sin(angle)*(arg-128)/64;
      health-=fabsf(arg-128)/10240;
    }
    break;
  case 1: // Turn
    if(arg>127)
    {
      angle+=M_PIl*(arg-127)/128;
      health-=fabsf(arg-127)/40960;
    }else{
      angle+=M_PIl*(arg-128)/128;
      health-=fabsf(arg-128)/40960;
    }
    break;
  case 2: // Jump (pointer)
    if(arg>127) // Always go back one more step so that +0 (which we don't have) would result in a loop
      pointer+=arg-128;
    else
      pointer+=arg-129;
    break;
  case 3: // Look
    writeVisual(mem, mempointer, arg);
    break;
  case 4: // Move mempointer
    if(arg>127)
      mempointer+=arg-127;
    else
      mempointer+=arg-128;
    if(mempointer>511) mempointer-=512;
    if(mempointer<0) mempointer+=512;
    break;
  case 5: // If
    if(mem[mempointer]!=arg) pointer++;
    break;
  case 6: // If not
    if(mem[mempointer]==arg) pointer++;
    break;
  case 7: // If more
    if(mem[mempointer]<=arg) pointer++;
    break;
  case 8: // If less
    if(mem[mempointer]>=arg) pointer++;
    break;
  case 9:  // Eat	MISSING
    break;
  case 10: // Attack	MISSING
    break;
  case 11: // Procreate	MISSING
    break;
  case 12: // Custom (send an event for plugins to handle)
    eventobj.name="CustomCOL";
    eventobj.integers.push_back(arg);
    eventobj.pointers.push_back(this);
    eventMgr=(eventManager*)pointers::getInstance()->getPointer("eventManager");
    eventMgr->triggerEvent(eventobj);
    break;
  case 13: // Custom arg (send an event for plugins to handle, and save for the Custom command)
    eventobj.name="CustomCOLarg";
    eventobj.integers.push_back(arg);
    eventobj.pointers.push_back(this);
    eventMgr=(eventManager*)pointers::getInstance()->getPointer("eventManager");
    eventMgr->triggerEvent(eventobj);
    break;
  default:
    log "Command not implemented: %i\n", col[pointer] endlog;
  }
  pointer++;
  if(pointer>=col_length) pointer-=col_length;
}

void creature::writeVisual(unsigned char* mem, int mempointer, int resolution)
{
  /* Loop through objects and get position in memory based on angle */
  /* We also need to assure that objects further away don't overwrite closer objects */
  Object** tmpobj=(Object**)pointers::getInstance()->getPointerLockWait("objects");
  if(!tmpobj) return; // Blind!
  Object* objiter=*tmpobj;
  if(mempointer+resolution>=512) resolution=512-mempointer-1;
  if(resolution<0) return; // We're probably trying to write the visual data at the very end of the memory, causing it to be set back to -1
  unsigned int distance[resolution+1];
  memset(distance, 255, sizeof(unsigned int)*(resolution+1));
  while(objiter)
  {
    float X,Y;
    objiter->getPosition(X, Y);
    /* Calculate angle of this object relative to 'this'. */
    float objangle;
    if(Y==0)
    {
      if(X>0)
        objangle=0;
      else
        objangle=M_PIl;
    }
    else if(X==0)
    {
      if(Y>0)
        objangle=M_PIl/2;
      else
        objangle=M_PIl*1.5;
    }
    else
    {
      objangle=atanf((Y-this->y)/(X-this->x));
      if(X-this->x<0) objangle=M_PIl+objangle;
      if(Y-this->y<0) objangle=M_PIl*2+objangle;
      if(objangle>=M_PIl*2) objangle-=M_PIl*2;
    }
    if(fabs(objangle-angle)<=1.5)
    {
      int angleindex=(int)(resolution*(objangle-angle+1.5)/3);
      /* Chceck if the distance to the object is less than  */
      /*   distance[resolution*angle/MAX_ANGLE]             */
      float thisdistance=sqrtf((Y-this->y)*(Y-this->y)+(X-this->x)*(X-this->x));
      if(thisdistance<distance[angleindex])
      {
        /* Store the image of this object in memory and note its distance */
        distance[angleindex]=(unsigned int)thisdistance;
        mem[mempointer+angleindex]=objiter->getType();
      }
    }
    objiter=objiter->next;
  }
}
