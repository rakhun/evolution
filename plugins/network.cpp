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
 *	(30 bytes total, float & int=4, char=1)
 * 2	bool		Server tells the client that the connection to the edge
 * 			requested was successful (1) or failed (0), in which
 * 			case the client will attempt the next edge provided.
 * 			Sort of the key to enter networkHandler(..)
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
#include "../creature.h"
#include "../log.h"

void* networkServer2(void*);

pointers* pointerhub;
int connections[4]; // We might need some mutexes here preventing that we send and receive at the same time on a connection

void networkHandler(int connection)
{
  unsigned char* col;
  std::vector<creature*>* creatures;
  creature* newcreature;
  unsigned char packettype;
  while(true)
  {
    if(recv(connections[connection], &packettype, 1, 0)<1) return;
    switch(packettype)
    {
    case 0:
      log "Error! packettype 0 should not appear here\n" endlog;
      return;
    case 1:
/*
 * 1    float x         Sending a creature across, same for client and server
 *      float y
 *      float angle
 *      u_int col_len
 *      u_char*x col
 *      u_int pointer
 *      u_char*512 mem
 *      int mempointer
 *      int health
 *      (30 bytes total, float & int=4, char=1)
 */
      float x, y, angle, health;
      unsigned int col_len, pointer;
      unsigned char mem[512];
      int mempointer;
      recv(connections[connection], &x, sizeof(float), 0);
      recv(connections[connection], &y, sizeof(float), 0);
      recv(connections[connection], &angle, sizeof(float), 0);
      recv(connections[connection], &col_len, sizeof(unsigned int), 0);
      col=(unsigned char*)malloc(col_len*sizeof(unsigned char)*2);
      recv(connections[connection], col, sizeof(unsigned char)*2*col_len, 0);
      recv(connections[connection], &pointer, sizeof(unsigned int), 0);
      recv(connections[connection], &mem, sizeof(unsigned char)*512, 0);
      recv(connections[connection], &mempointer, sizeof(int), 0);
      recv(connections[connection], &health, sizeof(float), 0);
      log "Received creature data:\nPosition: %f, %f\nAngle: %f\nCOL length: %i\nPointer: %i\nMempointer: %i\nHealth: %f\n", x, y, angle, col_len, pointer, mempointer, health endlog;
      newcreature=new creature(x, y, angle, col, col_len, pointer, mem, mempointer, health);
      creatures=(std::vector<creature*>*)pointerhub->getPointerLockWait("creatures");
      creatures->push_back(newcreature);
      pointerhub->unlockPointer("creatures");
      delete col;
      break;
    case 2:
      log "Error! packettype 2 should not appear here\n" endlog;
      return;
    default:
      printf("Faulty packettype %u\n", packettype);
    }
  }
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
  if(int r=connect(sock, (const struct sockaddr*)&sockaddr, sizeof(struct sockaddr_in)))
  {
    printf("failed (%i)\n", r);
    return false;
  }else{
    puts("Ok");
  }
  // *Negotiate edge
  char msg[2];
  char edge;
  char edgesoffered;
  while(true)
  {
    int r=recv(sock, msg, 1, 0);
    if(r<1){log "Bad return value! (%i)\n", r endlog; return 0;}
    log "Received packettype %i on socket %i\n", msg[0], sock endlog;
    if(msg[0]==0) // Receiving a list of available edges, answering with a request to connect to one.
    {
      int r=recv(sock, msg+1, 1, 0);
      if(r<1){log "Faulty packet!" endlog;return 0;}
      log "Received offers: %i\n", msg[1] endlog;
      edge=0;
      if(msg[1]&1 &&!connections[0]) edge=1;
      else if(msg[1]&2 &&!connections[1]) edge=2;
      else if(msg[1]&4 &&!connections[2]) edge=3;
      else if(msg[1]&8 &&!connections[3]) edge=4;
      connections[edge-1]=sock;
      log "connections[%i]=sock(%i)\n", edge-1, sock endlog;
      edgesoffered=msg[1];
      msg[0]=0;
      msg[1]=edge;
      send(sock, msg, 2, MSG_DONTWAIT);
      if(!edge){close(sock); return 0;}
    }else if(msg[0]==2) // Confirmation or decline of request to connect to an edge, trying another edge if declined
    {
      int r=recv(sock, msg+1, 1, 0);
      if(r<1) return 0;
      if(!(bool)msg[1])
      {
        if(edgesoffered&1 &&connections[0]==sock) edgesoffered-=1;
        else if(edgesoffered&2 &&connections[1]==sock) edgesoffered-=2;
        else if(edgesoffered&4 &&connections[2]==sock) edgesoffered-=4;
        else if(edgesoffered&8 &&connections[3]==sock) edgesoffered-=8;
        connections[edge-1]=0; // Couldn't use that edge, trying another
        edge=0;
        if(edgesoffered&1 &&!connections[0]) edge=1;
        else if(edgesoffered&2 &&!connections[1]) edge=2;
        else if(edgesoffered&4 &&!connections[2]) edge=3;
        else if(edgesoffered&8 &&!connections[3]) edge=4;
        connections[edge-1]=sock;
        msg[0]=0;
        msg[1]=edge;
        send(sock, msg, 2, MSG_DONTWAIT);
        if(!edge){close(sock); return 0;}
      }else{
        log "Link to edge %i confirmed\n", edge endlog;
        break;
      }
    }
  }
  networkHandler(edge-1);
  return 0;
}

void* networkServer(void* portstring)
{
  int sock=socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in sockaddr;
  sockaddr.sin_family=AF_INET;
  sockaddr.sin_addr.s_addr=INADDR_ANY;
  sockaddr.sin_port=htons(atoi((const char*)portstring));
  bind(sock, (const struct sockaddr*)&sockaddr, sizeof(struct sockaddr_in));
  listen(sock, 6);
  struct sockaddr_in clientinfo;
  socklen_t addrlen=sizeof(sockaddr_in);
  int client;
  while((client=accept(sock, (struct sockaddr*)&clientinfo, &addrlen))>=0)
  {
    pthread_create(new pthread_t, NULL, networkServer2, new int(client));
  }
  return 0;
}

void* networkServer2(void* socket)
{
  char edge=0;
  if(!connections[0]) edge+=1;
  if(!connections[1]) edge+=2;
  if(!connections[2]) edge+=4;
  if(!connections[3]) edge+=8;
  char msg[2];
  msg[0]=0;
  msg[1]=edge;
  write(*(int*)socket, msg, 2); // Sending available edges to the client
  while(true)
  {
    int r=recv(*(int*)socket, msg, 1, 0);
    if(r<1)
    {
      delete (int*)socket;
      return 0;
    }
    if(msg[0]==0)
    {
      int r=recv(*(int*)socket, msg+1, 1, 0);
      if(r<1)
      {
        delete (int*)socket;
        return 0;
      }
      //edge=0;
      if(msg[1]<5&&msg[1]>0&&!connections[msg[1]-1])
      {
        //*Accepted, handle it
        connections[msg[1]-1]=*(int*)socket;
        edge=msg[1];
        log "Accepting link request to edge %i\n", msg[1] endlog;
        msg[0]=2; // Confirm the request
        msg[1]=true;
        send(*(int*)socket, msg, 2, MSG_DONTWAIT);
        break;
      }else{
        msg[0]=2; // Decline the request, the client should request another port
        msg[1]=false;
        send(*(int*)socket, msg, 2, MSG_DONTWAIT);
      }
    }else{
      log "Error! Only packettype 0 should be received by this part of the server\n" endlog;
      delete (int*)socket;
      return 0;
    }
  }
  log "Server: Negotiated edge %i\n", edge-1 endlog;
  connections[edge-1]=*(int*)socket;
  delete (int*)socket;
  networkHandler(edge-1);
  return 0;
}

bool transferCreature(int id, char edge)
{
  log "Trying to transfer creature %i to edge %i\n", id, edge endlog;
  std::vector<creature*>* creatures=(std::vector<creature*>*)pointerhub->getPointerLockWait("creatures");
  if(!creatures) return false;
  unsigned char* col;
  unsigned int col_length, pointer;
  int mempointer;
  creatures->at(id)->getCOL(col, col_length);
  float x, y, angle, health;
  creatures->at(id)->getPosition(x, y);
  angle=creatures->at(id)->getAngle();
  health=creatures->at(id)->getLife();
  pointer=creatures->at(id)->getPointer();
  mempointer=creatures->at(id)->getMemPointer();
  unsigned int size=sizeof(float)*3+sizeof(unsigned int)*2+sizeof(int)*2+sizeof(unsigned char)*(512+col_length*2+1);
  void* msg=malloc(size);
  ((char*)msg)[0]=1;
  unsigned int pos=1;
  memcpy((void*)((size_t)msg+pos), &x, sizeof(float)); pos+=sizeof(float);
  memcpy((void*)((size_t)msg+pos), &y, sizeof(float)); pos+=sizeof(float);
  memcpy((void*)((size_t)msg+pos), &angle, sizeof(float)); pos+=sizeof(float);
  memcpy((void*)((size_t)msg+pos), &col_length, sizeof(unsigned int)); pos+=sizeof(unsigned int);
  memcpy((void*)((size_t)msg+pos), col, sizeof(unsigned char)*2*col_length); pos+=sizeof(unsigned char)*2*col_length;
  memcpy((void*)((size_t)msg+pos), &pointer, sizeof(unsigned int)); pos+=sizeof(unsigned int); // pointer
  pos+=sizeof(unsigned char)*512; // mem
  memcpy((void*)((size_t)msg+pos), &mempointer, sizeof(int)); pos+=sizeof(int); // mempointer
  memcpy((void*)((size_t)msg+pos), &health, sizeof(float)); pos+=sizeof(float);
/*
 * 1	float x		Sending a creature across, same for client and server
 *	float y
 *	float angle
 *	u_int col_len
 *	u_char*x col
 *	u_int pointer
 *	u_char*512 mem
 *	int mempointer
 *	int health
 *	(30 bytes total, float & int=4, char=1)
 */
  log "Sending message to transfer creature" endlog;
  send(connections[(unsigned int)edge], msg, size, MSG_DONTWAIT);
  free(msg);
  creatures->erase(creatures->begin()+id);
  pointerhub->unlockPointer("creatures");
  return true;
}

bool transferLeft(event eventobj)
{
  log "Attempting to send creature left..." endlog;
  if(!connections[0]) return false; // No other world connected to that edge
  log "Proceeding" endlog;
  if(eventobj.integers.size()<1) return false; // Erroneous event
  return transferCreature(eventobj.integers[0], 0);
}

bool transferTop(event eventobj)
{
  log "Attempting to send creature up..." endlog;
  if(!connections[1]) return false; // No other world connected to that edge
  log "Proceeding" endlog;
  if(eventobj.integers.size()<1) return false; // Erroneous event
  return transferCreature(eventobj.integers[0], 1);
}

bool transferRight(event eventobj)
{
  log "Attempting to send creature right..." endlog;
  if(!connections[2]) return false; // No other world connected to that edge
  log "Proceeding" endlog;
  if(eventobj.integers.size()<1) return false; // Erroneous event
  return transferCreature(eventobj.integers[0], 2);
}

bool transferBottom(event eventobj)
{
  log "Attempting to send creature down..." endlog;
  if(!connections[3]) return false; // No other world connected to that edge
  log "Proceeding" endlog;
  if(eventobj.integers.size()<1) return false; // Erroneous event
  return transferCreature(eventobj.integers[0], 3);
}

int handleArg(int argc, const char** argv, int& i)
{
  if(!strcmp(argv[i], "-connect")){
    if(i+1>=argc)
    {
      fputs("-connect: missing argument\n", stderr);
      exit(1);
    }
    i++;
    char* server=new char[strlen(argv[i])+1];
    strcpy(server, argv[i]);
    pthread_create(new pthread_t, NULL, networkConnect, server);
    return 1;
  }else if(!strcmp(argv[i], "-listen")){
    char* port;
    if(i+1>=argc)
    {
      port=(char*)"3526";
    }else{
      i++;
      port=new char[strlen(argv[i])+1];
      strcpy(port, argv[i]);
    }
    pthread_create(new pthread_t, NULL, networkServer, port);
    return 1;
  }else if(!strcmp(argv[i], "-help")){
    puts("-connect [server]:[port]          Connect to another virtual world to extend it geographically");
    puts("-listen [port]                    Listen for incoming connections to extend the virtual world");
    return -1; // = not handled/continue anyway
  }
  return -1;
}

extern "C" {
  void initplugin(pointers* pointersobj)
  {
    pointerhub=pointersobj;
    connections[0]=connections[1]=connections[2]=connections[3]=0;
    ((std::vector<int (*)(int, const char**, int&)>*)pointersobj->getPointer("argHandlers"))->push_back(handleArg);
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("warp top", transferTop);
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("warp bottom", transferBottom);
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("warp left", transferLeft);
    ((eventManager*)pointersobj->getPointer("eventManager"))->registerEvent("warp right", transferRight);
  }

  __attribute__((destructor))
  void endplugin()
  {
    if(connections[0])
    {
      shutdown(connections[0], 3);
      close(connections[0]);
    }
    if(connections[1])
    {
      shutdown(connections[1], 3);
      close(connections[1]);
    }
    if(connections[2])
    {
      shutdown(connections[2], 3);
      close(connections[2]);
    }
    if(connections[3])
    {
      shutdown(connections[3], 3);
      close(connections[3]);
    }
  }
}
