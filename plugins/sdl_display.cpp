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
#ifdef debug
#include <stdio.h>
#endif
#include <unistd.h>
#include <SDL/SDL.h>
#include <pthread.h>
#include "../pointers.h"
#include "../world.h"
#include "../creature.h"

pointers* pointerobj;

void draw(SDL_Surface* screen, SDL_Rect* bg)
{
  SDL_FillRect(screen, bg, 0x00E000);
  std::vector<creature*>* people=(std::vector<creature*>*)pointerobj->getPointerLock("creatures");
  if(!people) return;
  float x, y;
  for(unsigned int i=0; i<people->size(); i++)
  {
    people->at(i)->getPosition(x, y);
    SDL_Rect dummy; /// @todo This should be a picture or something, not just a square
    dummy.w=dummy.h=20;
    dummy.x=int(x)-10;
    dummy.y=int(y)-10;
    SDL_FillRect(screen, &dummy, 0xFFFFFF);
  }
  pointerobj->unlockPointer("creatures");
  SDL_Flip(screen);
}

void* SDL_Display(void* pointer)
{
  sleep(1); // Wait for other things to initialize, like handling -width and -height to set the world size
  pointerobj=(pointers*)pointer;
  world* worldobj=(world*)pointerobj->getPointer("world");
  if(!worldobj||SDL_Init(SDL_INIT_EVERYTHING)==-1) pthread_exit(0);

  SDL_Surface* screen=SDL_SetVideoMode(worldobj->width, worldobj->height, 24, SDL_HWSURFACE);
  SDL_WM_SetCaption("EvolutionBots", NULL);

  SDL_Rect bg;
  bg.x=bg.y=0;
  bg.w=worldobj->width;
  bg.h=worldobj->height;

  draw(screen, &bg);
  SDL_Event event;
  while(true)
  {
    while(SDL_PollEvent(&event))
    {
      if(event.type==SDL_QUIT)
      {
        SDL_Quit();
        exit(0);
      }
//    if(event.type==SDL_VIDEOEXPOSE) // Doesn't seem to be of any use with Compiz, but what about without it?
//    {
//      draw(screen, worldobj->width, worldobj->height);
//    }
    }
    draw(screen, &bg);
    usleep(200000);
  }
  SDL_Quit();
  pthread_exit(0);
}

extern "C" {
  void initplugin(pointers* pointerobj)
  {
    #ifdef debug
    world* worldobj=(world*)pointerobj->getPointer("world");
    printf("sdl_display: The size of the world is %ix%i pixels\n", worldobj->width, worldobj->height);
    #endif

    pthread_t thread;
    pthread_create(&thread, NULL, SDL_Display, pointerobj);
  }
}
