#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

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
    SOCKET servSock, clntSock;      // ���� �� ��ü�� ������ ������ ��� ����ü.
    SOCKADDR_IN servAddr, clntAddr; // IP �ּ�, TCP/UDP ��Ʈ �ּҿ� ���� ������ ��� ����ü.

    char message[BUFSIZE];
    int strLen;
    int fromLen, nRcv;
    int portNumber;

    //if (argc != 2) {
    //    printf("ERROR : PORT NUMBER NEEDED!\n");
    //    exit(1);
    //}

    printf("ENTER SERVER PORT NUMBER : ");
    scanf("%d", &portNumber);

    // ws2_32.dll ���� �ʱ�ȭ.
    // MAKEWORD ��ũ�� �̿�, 2.2 ���� ����.
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("Load Winsock 2.2 DLL Error");


    // socket()�� ���� ������ ����. TCP, UDP �� �⺻���� ���� ����.
    // ù��° ���ڴ� AF_INET �Ǵ� PF_INET. ���� �ּ� ü�� �׸��� �������� ü�踦 �ǹ�.
    // �ι�° ���ڴ� TCP(SOCK_STREAM) ����� �������, UDP(SOCK_DGRAM) ����� ������� ����.
    // ����° ���ڴ� 0�� ���, Auto �� �ڵ����� �������� ����.
    servSock = socket(PF_INET, SOCK_STREAM, 0);

    if (servSock == INVALID_SOCKET)
        ErrorHandling("socket error!");


    // bind()�� ���� ������ IP, Port ���.
    memset(&servAddr, 0, sizeof(SOCKADDR_IN));
    servAddr.sin_family = AF_INET;                          // IPv4 ���ͳ� �������� 

    // htons, htonl �Լ��� �����͸� ��Ʈ��ũ ����Ʈ ������ ��ȯ�����ִ� ����.
    // htons�� short integer�� ��Ʈ��ũ ����Ʈ ������,
    // htonl�� long interger�� ��Ʈ��ũ ����Ʈ ������ ��ȯ.
    servAddr.sin_port = htons(portNumber);                // ��Ʈ ��ȣ ����.
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);    // 32��Ʈ IPv4 �ּ�. INADDR_ANY�� ������ IP �ּҸ� �ڵ����� ã�� ���Խ����ִ� �Լ�.

    if (bind(servSock, (void*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("bind error!");

    // listen()�� ���� Ŭ���̾�Ʈ connect() ���� ��û�� Ȯ���ϵ��� ���ش�. ��û Ȯ�� �� accept() �� �Ѱ��ش�.
    // �� ��° ���ڴ� �ִ� ���� Ƚ��.
    if (listen(servSock, MAXIMUM_CONNECTION_COUNT) == SOCKET_ERROR)
        ErrorHandling("listen error!");

    fromLen = sizeof(clntAddr);

    // accept()�� ���� Ŭ���̾�Ʈ ��û�� ó��, Ŭ���̾�Ʈ���� ����� ���� ���� ����.
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
        // recv() �� �������κ��� �����͸� ����.
        nRcv = recv(clntSock, message, sizeof(message) - 1, 0);

        if (nRcv == SOCKET_ERROR) {
            ErrorHandling("receive error");
            break;
        }
        message[nRcv] = '\0';

        printf("MESSAGE : %s\n", message);
    }

    closesocket(clntSock);

    // ������ ������ ť�� ���� �����͸� ó��.
    WSACleanup();

    _getch();

    return 0;
}