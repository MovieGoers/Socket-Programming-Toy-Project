#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUFSIZE 1024
#define MAXIMUM_CONNECTION_COUNT 2

void ErrorHandling(char* message) {
    printf("%s\n", message);
    exit(1);
}

int main(int argc, char** argv) {

    WSADATA wsaData;
    SOCKET servSock, clntSock;      // 소켓 그 자체와 관련한 정보를 담는 구조체.
    SOCKADDR_IN servAddr, clntAddr; // IP 주소, TCP/UDP 포트 주소에 대한 정보를 담는 구조체.

    char message[BUFSIZE];
    int strLen;
    int fromLen, nRcv;

    //if (argc != 2) {
    //    printf("ERROR : PORT NUMBER NEEDED!\n");
    //    exit(1);
    //}


    // ws2_32.dll 파일 초기화.
    // MAKEWORD 매크로 이용, 2.2 버전 지정.
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("Load Winsock 2.2 DLL Error");


    // socket()을 통해 소켓을 생성. TCP, UDP 등 기본적인 설정 구성.
    // 첫번째 인자는 AF_INET 또는 PF_INET. 각각 주소 체계 그리고 프로토콜 체계를 의미.
    // 두번째 인자는 TCP(SOCK_STREAM) 방식을 사용할지, UDP(SOCK_DGRAM) 방식을 사용할지 결정.
    // 세번째 인자는 0일 경우, Auto 즉 자동으로 프로토콜 선정.
    servSock = socket(PF_INET, SOCK_STREAM, 0);

    if (servSock == INVALID_SOCKET) 
        ErrorHandling("socket error!");
    

    // bind()를 통해 서버의 IP, Port 등록.
    memset(&servAddr, 0, sizeof(SOCKADDR_IN));
    servAddr.sin_family = AF_INET;                          // IPv4 인터넷 프로토콜 

    // htons, htonl 함수는 데이터를 네트워크 바이트 순서로 변환시켜주는 역할.
    // htons는 short integer를 네트워크 바이트 순서로,
    // htonl는 long interger를 네트워크 바이트 순서로 변환.
    servAddr.sin_port = htons(8000);                // 포트 번호 설정.
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);    // 32비트 IPv4 주소. INADDR_ANY는 서버의 IP 주소를 자동으로 찾아 대입시켜주는 함수.

    if (bind(servSock, (void*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("bind error!");

    // listen()을 통해 클라이언트 connect() 접속 요청을 확인하도록 해준다. 요청 확인 시 accept() 에 넘겨준다.
    // 두 번째 인자는 최대 연결 횟수.
    if (listen(servSock, MAXIMUM_CONNECTION_COUNT) == SOCKET_ERROR)
        ErrorHandling("listen error!");

    fromLen = sizeof(clntAddr);

    // accept()를 통해 클라이언트 요청을 처리, 클라이언트와의 통신을 위한 소켓 생성.
    clntSock = accept(servSock, (void*)&clntAddr, &fromLen);
    if (clntSock == INVALID_SOCKET) {
        ErrorHandling("accept error!");
    }
    else {
        printf("%s Connected..\n", inet_ntoa(clntAddr.sin_addr));
        printf("STARTING..\n");
    }

    closesocket(servSock);

    while (1) {
        printf("RECEIVING MESSAGE..\n");
        
        // recv() 는 소켓으로부터 데이터를 수신.
        nRcv = recv(clntSock, message, sizeof(message) - 1, 0);

        if (nRcv == SOCKET_ERROR) {
            ErrorHandling("receive error");
            break;
        }
        message[nRcv] = '\0';

        printf("MESSAGE : %s\n", message);
    }

    closesocket(clntSock);

    // 소켓이 닫히고 큐에 남은 데이터를 처리.
    WSACleanup();

    _getch();

    return 0;
}
