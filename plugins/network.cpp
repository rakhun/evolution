/*
 * Network messages:
 * ID	Values		Uses
 * 0	char edge	Server tells the client which edges are available.
 *			Client responds with value 0 (cancel) or which edge
 *			to connect with
 * 1	float x		Sending a creature across, same for client and server
 *	float y
 *	float angle
 *	u_int col_len
 *	u_char*x col
 *	u_int pointer
 *	u_char*512 mem
 *	int mempointer
 *	int health
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include "../pointers.h"
#include "../eventmanager.h"

pointers* pointerhub;
int connections[4]; // We might need some mutexes here preventing that we send and receive at the same time on a connection

void networkHandler(int connection)
{

}

void* networkConnect(void* voidserver)
{
  char* server=(char*)voidserver;
  char* tmp=strchr(server, ':');
  int port;
  if(tmp)
  {
    tmp[0]=0;
    port=atoi(tmp+1);
  }else{
    port=3526;
  }

  printf("Connecting to server '%s' on port %i... ", server, port);
  int sock=socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in sockaddr;
  sockaddr.sin_family=AF_INET;
  sockaddr.sin_addr.s_addr=inet_addr(server);
  sockaddr.sin_port=htons(port);
  if(int r=connect(sock, (const struct sockaddr*)&sockaddr, sizeof(sockaddr)))
  {
    printf("failed (%i)\n", r);
    return false;
  }else{
    puts("Ok");
  }
  // *Negotiate edge
  char msg[2];
  int edge;
  while(true)
  {
    int r=recv(sock, msg, 1, 0);
    if(r<1) return 0;
    if(msg[0]==0)
    {
      int r=recv(sock, msg+1, 1, 0);
      if(r<1) return 0;
      edge=0;
      if(msg[1]&1 &&!connections[0]) edge=1;
      else if(msg[1]&2 &&!connections[1]) edge=2;
      else if(msg[1]&4 &&!connections[2]) edge=3;
      else if(msg[1]&8 &&!connections[3]) edge=4;
      msg[0]=0;
      msg[1]=edge;
      write(sock, msg, 2);
      if(!edge){close(sock); return 0;}
      break;
    }
  }
  connections[edge-1]=sock;
  networkHandler(edge-1);
  // *Hand over control to a client/server independant function to handle transfer requests
}

void* networkServer(void* serverdata)
{
  int sock=socket(AF_INET, SOCK_STREAM, 0);
  // *Setup the socket and listen for a connection
  char edges=0;
  if(!connections[0]) edges+=1;
  if(!connections[1]) edges+=2;
  if(!connections[2]) edges+=4;
  if(!connections[3]) edges+=8;
  char msg[2];
  msg[0]=0;
  msg[1]=edges;
  write(sock, msg, 2);
}

bool transferCreature(event eventobj)
{
  printf("network.so: Received event to transfer a creature on the %s, but networking is so far just a stub\n", eventobj.name+5);
}

int handleArg(int argc, const char** argv, int &i)
{
  if(!strcmp(argv[i], "-connect")){
    if(i+1>=argc)
    {
      fputs("-connect: missing argument\n", stderr);
      return 0;
    }
    i++;
    char* server=new char[strlen(argv[i])+1];
    strcpy(server, argv[i]);
    pthread_create(new pthread_t, NULL, networkConnect, server);
    return 0;
  }else if(!strcmp(argv[i], "-listen")){
    if(i+1>=argc)
    {
      fputs("-listen: missing argument\n", stderr);
      return 0;
    }
    i++;
    fputs("-listen: not yet implemented\n", stderr);
    return 0;
  }else if(!strcmp(argv[i], "-help")){
    puts("-connect [server]:[port]          Connect to another virtual world to extend it geographically");
    puts("-listen [port]                    Listen for incoming connections to extend the virtual world");
  }
  return -1; // = not handled/continue anyway
}

extern "C" {
  void initplugin(pointers* pointersobj)
  {
    pointerhub=pointersobj;
    connections[0]=connections[1]=connections[2]=connections[3]=0;
    ((std::vector<int (*)(int, const char**, int&)>*)pointersobj->getPointer("argHandlers"))->push_back(handleArg);
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("warp top", transferCreature);
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("warp bottom", transferCreature);
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("warp left", transferCreature);
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("warp right", transferCreature);
  }
}
