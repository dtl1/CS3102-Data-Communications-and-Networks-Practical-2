test: clientTest serverTest

clientTest: clientTest.c UdpSocket.h client.h
	gcc -o clientTest clientTest.c UdpSocket.c client.c

serverTest: serverTest.c UdpSocket.h server.h
	gcc -o serverTest serverTest.c UdpSocket.c server.c