
int
nonBlocking(int fd);

int 
idle(UdpSocket_t *local, UdpSocket_t *remote);

int 
recieveData(UdpSocket_t *local, UdpSocket_t *remote);

UdpBuffer_t* 
createPacket(int size, uint8_t content);