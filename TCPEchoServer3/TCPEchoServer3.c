/*TCPEchoServer <Server Port>*/
#include <stdio.h>	// printf()
#include <sys/socket.h>	// socket(),bind(),send(),recv()
#include <arpa/inet.h>	// sockaddr_in(),inet_addr(),htons()
#include <stdlib.h>	// atoi()
#include <string.h>	// memset(),strlen()
#include <unistd.h>	// close()

#define MAXPENDING 4	//number of Max pending of request
#define RCVBUFSIZE 1	//size of receive buffer

int HandleTCPClient(int clntSocket);	//Echo function

int main(int argc, char *argv[]){
	unsigned short echoServPort;
	int servSock;
	int clntSock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;
	unsigned int clntLen;

	echoServPort = atoi(argv[1]);

// make TCP socket()
	servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(servSock == -1){
		printf("make socket error\n");
		exit(1);
	}
	printf("PF_INET:%d SOCK_STREAM:%d IPPROTO_TCP:%d\n", PF_INET, SOCK_STREAM, IPPROTO_TCP);

//make address structure of local
	memset(&echoServAddr, 0, sizeof(echoServAddr));
		//fill memory with the constant byte (0)
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);	//use wild card
	echoServAddr.sin_port = htons(echoServPort);

//bind to local address
	if((bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)))==-1){
		printf("bind error\n");
		exit(1);
	}
//listen
	if((listen(servSock, MAXPENDING))==-1){
		printf("listen error\n");
		exit(1);
	}

//infinite loop
	while(1){
		clntLen = sizeof(echoClntAddr);
		if((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen))==-1){
			printf("accept error\n");
			exit(1);
		}
		printf("Handling client %s clntSock:%d \n", inet_ntoa(echoClntAddr.sin_addr), clntSock);
		HandleTCPClient(clntSock);
	}
}
int HandleTCPClient(int cs){
	char echoBuffer[RCVBUFSIZE];
	int recvMsgSize;
	memset(&echoBuffer,'\0',sizeof(echoBuffer));
	recvMsgSize = recv(cs, echoBuffer, RCVBUFSIZE, 0);	//receive Msg from client
	if(recvMsgSize == -1){
		printf("receive error\n");
		exit(1);
	}
	else printf("recvMsgsize:%d\n", recvMsgSize);
	// send Msg to server
	while(recvMsgSize > 0){
		printf("%c", echoBuffer[0]);
		send(cs, echoBuffer, recvMsgSize, 0);
		recvMsgSize = recv(cs, echoBuffer, RCVBUFSIZE, 0);
		sleep(1);
	}
	if((close(cs))==-1){
		printf("close error\n");
		exit(1);
	}
	printf("\n");
	return 0;
}

