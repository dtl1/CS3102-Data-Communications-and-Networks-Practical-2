/*
  Test program to test client api
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
#define DATATRIES ((int) 10)

//error functions
#define ERROR(_s) fprintf(stderr, "%s\n", _s)
void perror(const char *s);


int
main(int argc, char *argv[]){


  UdpSocket_t *local, *remote;
  long long responseTime;

  if (argc != 2) {
    ERROR("usage: clientTest <hostname>");
    exit(0);
  }

  printf("Creating Sockets... \n");
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

  printf("Attempting to establish connection with the server on %s... \n", argv[1]);
    responseTime = establishHandShake(local,remote);
    if(responseTime > 0)
      printf("\t...Server connected in %lldms\n", responseTime);
    else
      printf("\t...Server connected in less than 1ms\n");
  printf("Connection established. \n\n");

  
  int c = 0;
  while (c < DATATRIES){
  printf("Sending some data to the server... \n");
    responseTime = sendData(local, remote);
    if(responseTime > 0)
      printf("\t...Server acknowledged the data in %lldms\n", responseTime);
    else
      printf("\t...Server acknowledged the data in less than 1ms\n");
    c++;
  }

  printf("\nSending request to terminate connection... \n");
    responseTime = terminateHandShake(local,remote);
    if(responseTime > 0)
      printf("\t...Server agreed in %lldms\n", responseTime);
    else
      printf("\t...Server agreed in less than 1ms\n");


  closeUdp(local);
  closeUdp(remote);

  printf("Connection terminated. \n\n");

 return 0;
}

