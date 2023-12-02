#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
// � ������ �������, � ������� Linker, � ������ Additional Dependancies ������� Ws2_32.lib
int __cdecl main(void)
{
    WSADATA wsaData;
    SOCKET ListenSocket,ClientSocket; // ���������� ����� � ����� ��� ��������
    sockaddr_in ServerAddr; // ��� ����� ����� �������
    int err, maxlen = 512; // ��� ������ � ������ �������
    char* recvbuf=new char[maxlen]; // ����� ������
    char* result_string=new char[maxlen]; // ����� ��������
    // Initialize Winsock
    WSAStartup(MAKEWORD(2,2), &wsaData);
    // Create a SOCKET for connecting to server
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Setup the TCP listening socket
    ServerAddr.sin_family=AF_INET;
    ServerAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    ServerAddr.sin_port=htons(12345);
    err = bind( ListenSocket, (sockaddr *) &ServerAddr, sizeof(ServerAddr));

    if (err == SOCKET_ERROR) {
        printf("bind failed: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    err = listen(ListenSocket, 50);

    if (err == SOCKET_ERROR) {
        printf("listen failed: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    err = recv(ClientSocket, recvbuf, maxlen, 0);

    if (err > 0) {
        recvbuf[err]=0;
        printf("Received query: %s\n", (char* )recvbuf);
        // ��������� ���������
        int result=42;
        sprintf(result_string,"OK %d\n",result);
        // ���������� ��������� �� ������
        send( ClientSocket, result_string, strlen(result_string), 0 );
        printf("Sent answer: %s\n", result_string);
    }
    else if (err == 0)
        printf("Connection closing...\n");
    else {
        printf("recv failed: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
    // shutdown the connection since we're done
    closesocket(ClientSocket);
}
