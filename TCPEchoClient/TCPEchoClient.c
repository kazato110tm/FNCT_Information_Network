/*TCPEchoClient <Server IP> <Echo Word> <Echo Port>*/
#include <stdio.h>	// printf()
#include <sys/socket.h>	// socket(),connect(),send(),recv()
#include <arpa/inet.h>	// sockaddr_in(),inet_addr(),htons()
#include <stdlib.h>	// atoi()
#include <string.h>	// memset(),strlen()
#include <unistd.h>	// close()

#define RCVBUFSIZE 32	// size of receive buffer

int main(int argc, char *argv[]){
	char *servIP;
	char *echoString;
	unsigned short echoServPort;

	int sock;				// socket descriptor
	struct sockaddr_in echoServAddr;

	unsigned int echoStringLen;
	int sendMsgSize;
	int bytesRcvd, totalBytesRcvd;
	char echoBuffer[RCVBUFSIZE];
	
	servIP = argv[1];
	echoString = argv[2];
	echoServPort = atoi(argv[3]);
// make TCP socket
	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))==-1){
		printf("make socket error\n");
		exit(1);
	}
// make address structure of server
	memset(&echoServAddr, 0, sizeof(echoServAddr));
		// fill memory with the constant byte (0)
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = inet_addr(servIP);
	echoServAddr.sin_port = htons(echoServPort);
		// converts the unsigned integer hostshort from
		// host byte order to network byte order (host to network short)
// connect to EchoServer
	if((connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)))==-1){
		printf("connect error\n");
		exit(1);
	}
	echoStringLen = strlen(echoString);	// length of input data(argv[2])
// send String to EchoServer without last char 0x00
	sendMsgSize = send(sock, echoString, echoStringLen, 0);
	if(sendMsgSize == -1){
		printf("send error\n");
		exit(1);
	}
	else printf("sendMsgSize:%d\n",sendMsgSize);
// receive same String from EchoServer
	totalBytesRcvd = 0;
	printf("PF_INET:%d SOCK_STREAM:%d IPPROTO_TCP:%d sock:%d\n", PF_INET, SOCK_STREAM, IPPROTO_TCP, sock);
	printf("Received:");
	while(totalBytesRcvd < echoStringLen){
		bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0);
		totalBytesRcvd += bytesRcvd;
		echoBuffer[bytesRcvd] = '\0';	//add last char 0x00
		printf("%s\n",echoBuffer);
		printf("bytesRcvd:%d\n",bytesRcvd);
	}
	printf("\n");
	if((close(sock))==-1){
		printf("close\n");
		exit(1);
	}
	return 0;
}

