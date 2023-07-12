/*
  Server api to listen for client data
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "UdpSocket.h"
#include "server.h"

#define MY_PORT   ((uint16_t) 21929) // from 'id -u'
#define PACKET_SIZE      ((int) 16)
#define SLEEPTIME ((uint32_t) 1) // 1 second

#define ESTABLISHCODE (uint8_t) 1
#define TERMINATECODE (uint8_t) 2
#define ACKNOWLEDGECODE (uint8_t) 3
#define EMPTYNUMBER (uint8_t) 0



int idle(UdpSocket_t *local, UdpSocket_t *remote){

    //create and initialse empty response packet
    UdpBuffer_t* resPacket = malloc(PACKET_SIZE);
    uint8_t* emptyPacketBytes, emptyNumber;
    emptyNumber = EMPTYNUMBER;
    emptyPacketBytes = &emptyNumber;
    resPacket->n = PACKET_SIZE;
    resPacket->bytes = emptyPacketBytes;

    //create and initialse establishment packet
    UdpBuffer_t* establishPacket = malloc(PACKET_SIZE);
    uint8_t* estpacketBytes, estnumber;
    estnumber = ESTABLISHCODE;
    estpacketBytes = &estnumber;
    establishPacket->n = PACKET_SIZE;
    establishPacket->bytes = estpacketBytes;

    //create and initialse acknowledgement packet
    UdpBuffer_t* ACKPacket = malloc(PACKET_SIZE);
    uint8_t* ackpacketBytes, packetContent;
    packetContent = ACKNOWLEDGECODE;
    ackpacketBytes = &packetContent;
    ACKPacket->n = PACKET_SIZE;
    ACKPacket->bytes = ackpacketBytes;

    //create and initialse termination packet
    UdpBuffer_t* terminatePacket = malloc(PACKET_SIZE);
    uint8_t* terpacketBytes, ternumber;
    ternumber = TERMINATECODE;
    terpacketBytes = &ternumber;
    terminatePacket->n = PACKET_SIZE;
    terminatePacket->bytes = terpacketBytes;


    int terminate = 0;

    while(!terminate){
      resPacket->bytes = emptyPacketBytes;

      int res = recvUdp(local,remote,resPacket);

    if(res!=-1){
      int resContent = *resPacket->bytes;

      if(resContent == ESTABLISHCODE){
        printf("Request to establish connection received ... \n");
        sendUdp(local,remote,establishPacket);
        printf("\t ... Sending response\n\n");
      } else if (resContent == TERMINATECODE){
        printf("\n\nRequest to terminate connection received ... \n");
        sendUdp(local,remote,terminatePacket);
        terminate = 1;
        printf("\t ... Sending response\n");
      } else{
        printf("DATA transfer received ...\n");
        sendUdp(local,remote,ACKPacket);
        printf("\t ... Sending ACK\n");
      }
      res = -1;
    }
      
    }


}



//sets i/o to be non-blocking
int
nonBlocking(int fd)
{
  int r, flags = O_NONBLOCK;

  if ((r = fcntl(fd, F_SETFL, flags)) < 0) {
    perror("setAsyncFd(): fcntl() problem");
    exit(0);
  }

  return r;
}

