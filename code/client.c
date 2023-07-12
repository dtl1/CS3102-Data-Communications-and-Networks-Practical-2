/*
  Client api for sending data to the server
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
#include<sys/time.h>


#define PACKET_SIZE      ((int) 16)
#define RETRANSMITTIMEOUTMS ((int) 200) //200ms timeout as per spec

#define ESTABLISHCODE (uint8_t) 1
#define TERMINATECODE (uint8_t) 2
#define ACKNOWLEDGECODE (uint8_t) 3
#define EMPTYNUMBER (uint8_t) 0
#define DATA (uint8_t) 123



UdpBuffer_t* createPacket(int size, uint8_t content){
    
    //create and initialse packet
    UdpBuffer_t* packet = malloc(size);
    uint8_t* packetBytes, packetContent;
    packetContent = content;
    packetBytes = &packetContent;
    packet->n = size;
    packet->bytes = packetBytes;

    return packet;
}


long long establishHandShake(UdpSocket_t *local, UdpSocket_t *remote){

    UdpBuffer_t* estPacket = malloc(PACKET_SIZE);
    uint8_t* packetBytes, number;
    number = ESTABLISHCODE;
    packetBytes = &number;
    estPacket->n = PACKET_SIZE;
    estPacket->bytes = packetBytes;

    int establish = ESTABLISHCODE;
    return idleRQ(local, remote, estPacket, ESTABLISHCODE);
  
}


int sendData(UdpSocket_t *local, UdpSocket_t *remote){

    UdpBuffer_t* DATAPacket = malloc(PACKET_SIZE);
    uint8_t* packetBytes, number;
    number = DATA;
    packetBytes = &number;
    DATAPacket->n = PACKET_SIZE;
    DATAPacket->bytes = packetBytes;


    int acknowledge = ACKNOWLEDGECODE;
    return idleRQ(local, remote, DATAPacket, acknowledge);


}

int terminateHandShake(UdpSocket_t *local, UdpSocket_t *remote){
  
    UdpBuffer_t* terPacket = malloc(PACKET_SIZE);
    uint8_t* packetBytes, number;
    number = TERMINATECODE;
    packetBytes = &number;
    terPacket->n = PACKET_SIZE;
    terPacket->bytes = packetBytes;

    int terminate = TERMINATECODE;
    return idleRQ(local, remote, terPacket, terminate);

}


long long idleRQ(UdpSocket_t *local, UdpSocket_t *remote, UdpBuffer_t* sendPacket, uint8_t recPacketCodeCheck){
    UdpBuffer_t* ACKPacket = createPacket(PACKET_SIZE, EMPTYNUMBER);

    long long sendTime;
    long long diffTime;
    long long getTime;
    int acknowledged = 0;


    while(!acknowledged){


      //send packet
      sendUdp(local,remote,sendPacket);
      sendTime = timeInMilliseconds();

      diffTime = 0;
      while(diffTime < RETRANSMITTIMEOUTMS && !acknowledged){
        
        //check for response acknowledgement packet    
        int res = recvUdp(local,remote,ACKPacket);

        int resContent = *ACKPacket->bytes;

        if((res!=-1 && resContent == recPacketCodeCheck) || resContent == TERMINATECODE){ //check packet for specified code
          acknowledged = 1;
        }

        getTime = timeInMilliseconds();


        diffTime = getTime - sendTime;
      }

        
      if(!acknowledged){
        if(recPacketCodeCheck == 3)
          printf("\t...%ims timeout reached, re-transmitting data... \n", RETRANSMITTIMEOUTMS);
        else
          printf("\t...server didnt respond within %ims, re-attempting handshake... \n", RETRANSMITTIMEOUTMS);
      }
    }

    return diffTime;

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

//gets time sinc epoch in milliseconds
long long timeInMilliseconds(void) {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

