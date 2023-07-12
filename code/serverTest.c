/*
  Test program to test server api
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "UdpSocket.h"
#include "client.h"
#include "server.h"

#define MY_PORT   ((uint16_t) 21929) // from 'id -u'


//error functions
#define ERROR(_s) fprintf(stderr, "%s\n", _s)
void perror(const char *s);


int
main(int argc, char *argv[]){


  UdpSocket_t *local, *remote;

  printf("\nCreating Sockets... \n");

  if (argc != 2) {
    ERROR("usage: serverTest <hostname>");
    exit(0);
  }

  if ((local = setupUdpSocket_t((char *) 0, MY_PORT)) == (UdpSocket_t *) 0) {
    ERROR("problem setting up local socket");
    exit(0);
  }

  if ((remote = setupUdpSocket_t(argv[1], MY_PORT)) == (UdpSocket_t *) 0) {
    ERROR("remote hostname/port problem");
    exit(0);
  }

  if (openUdp(local) < 0) {
    ERROR("openUdp() problem");
    exit(0);
  }

  //set non-blocking i/o
  if (nonBlocking(local->sd) < 0) {
    ERROR("nonBlocking(local->sd) problem");
    exit(0);
  }
  printf("\t...Sockets successfully created.\n\n");
  
  printf("Listening for packets from client on %s... \n", argv[1]);
  idle(local,remote);


  closeUdp(local);
  closeUdp(remote);

  printf("Connection terminated. \n\n");


  return 0;

}
