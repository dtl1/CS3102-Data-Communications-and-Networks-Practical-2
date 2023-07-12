/*
  Client api for sending data to the server
*/
int
nonBlocking(int fd);

long long 
timeInMilliseconds(void);

long long 
establishHandShake(UdpSocket_t *local, UdpSocket_t *remote);

int 
terminateHandShake(UdpSocket_t *local, UdpSocket_t *remote);

int 
sendData(UdpSocket_t *local, UdpSocket_t *remote);


UdpBuffer_t* 
createPacket(int size, uint8_t content);

long long 
idleRQ(UdpSocket_t *local, UdpSocket_t *remote, UdpBuffer_t* sendPacket, uint8_t recPacketCodeCheck);