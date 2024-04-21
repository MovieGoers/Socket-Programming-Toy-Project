#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUFSIZE 1024

void ErrorHandling(char* message) {
    printf("%s\n", message);
    exit(1);
}

int main(int argc, char** argv) {

    WSADATA wsaData;
    SOCKET clntSock;      // 소켓 그 자체와 관련한 정보를 담는 구조체.
    SOCKADDR_IN servAddr; // IP 주소, TCP/UDP 포트 주소에 대한 정보를 담는 구조체.

    char sAddr[15];
    int sPort;
    int nRcv;
    unsigned int Addr;
    char message[BUFSIZE];

    struct hostent* host;

    printf("ENTER SERVER ADDRESS : ");
    gets(sAddr);
    printf("ENTER PORT NUMBER : ");
    gets(message);
    sPort = atoi(message);

    // ws2_32.dll 파일 초기화.
    // MAKEWORD 매크로 이용, 2.2 버전 지정.
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("Load Winsock 2.2 DLL Error");


    // socket()을 통해 소켓을 생성. TCP, UDP 등 기본적인 설정 구성.
    // 첫번째 인자는 AF_INET 또는 PF_INET. 각각 주소 체계 그리고 프로토콜 체계를 의미.
    // 두번째 인자는 TCP(SOCK_STREAM) 방식을 사용할지, UDP(SOCK_DGRAM) 방식을 사용할지 결정.
    // 세번째 인자는 0일 경우, Auto 즉 자동으로 프로토콜 선정.
    clntSock = socket(AF_INET, SOCK_STREAM, 0);

    if (clntSock == INVALID_SOCKET) 
        ErrorHandling("socket error!");
    

    // bind()를 통해 서버의 IP, Port 등록.
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;                          // IPv4 인터넷 프로토콜 
    servAddr.sin_port = htons(sPort);                // 포트 번호 설정.
    servAddr.sin_addr.s_addr = inet_addr(sAddr);    // 32비트 IPv4 주소. INADDR_ANY는 서버의 IP 주소를 자동으로 찾아 대입시켜주는 함수.

    if (connect(clntSock, (void*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        ErrorHandling("connect error");
    }
    else {
        printf("CONNECTED!\n");
    }

    while (1) {
        printf("ENTER MESSAGE : ");
        gets(message);

        send(clntSock, message, (int)strlen(message), 0);
        printf("MESSAGE SENT\n");
    }

    closesocket(clntSock);

    // 소켓이 닫히고 큐에 남은 데이터를 처리.
    WSACleanup();

    _getch();

    return 0;
}
