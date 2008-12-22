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

void draw(SDL_Surface* screen, int worldwidth, int worldheight)
{
  SDL_Rect bg;
  bg.x=0; bg.y=0;
  bg.w=worldwidth;
  bg.h=worldheight;

  SDL_Rect dummy; /// @todo This should be a picture or something, not just a square
  dummy.w=dummy.h=20;

  SDL_FillRect(screen, &bg, 0x00E000);
  std::vector<creature*>* people=(std::vector<creature*>*)pointerobj->getPointer("creatures");
  if(!people) return;
  for(unsigned int i=0; i<people->size(); i++)
  {
    people->at(i)->getPosition((int&)dummy.x, (int&)dummy.y);
    SDL_FillRect(screen, &dummy, 0xFFFFFF);
  }
  SDL_Flip(screen);
}

void* SDL_Display(void* pointer)
{
  pointerobj=(pointers*)pointer;
  world* worldobj=(world*)pointerobj->getPointer("world");
  if(!worldobj||SDL_Init(SDL_INIT_EVERYTHING)==-1) pthread_exit(0);

  SDL_Surface* screen=SDL_SetVideoMode(worldobj->width, worldobj->height, 24, SDL_HWSURFACE);
  SDL_WM_SetCaption("EvolutionBots", NULL);

  draw(screen, worldobj->width, worldobj->height);
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
    draw(screen, worldobj->width, worldobj->height);
    sleep(0.1);
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

    pthread_create(new pthread_t(), NULL, SDL_Display, pointerobj);
  }
}
