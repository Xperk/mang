#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#define DEFAULT_BUFLEN 512
using namespace std;

int main(int argc, char **argv)
{
    // Khoi tao winsock
    WSADATA wsaData;
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    char connectedClient[256];
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
    cout << "WSAStartup OK" << endl;

    // Khoi tao socket
    SOCKET sc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sc == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    cout << "ConnectSocket = socket OK" << endl;

    // Thiet lap dia chi IP va cong svr
    int port = 81;
    const char *IP = "127.0.0.1"; //"10.170.72.81"
    SOCKADDR_IN svrAddr;
    int svrAddrLen = sizeof(svrAddr);
    svrAddr.sin_family = AF_INET;
    svrAddr.sin_port = htons(port);
    svrAddr.sin_addr.s_addr = inet_addr(IP);

    // Nhap du lieu
    string studentInfo;
    cout<<"Nhap ten va ma sinh vien(Ten va ma sinh vien cach nhau boi dau ','): ";
    getline(cin,studentInfo);

    // Gui du lieu tu client den server
    iResult = sendto(sc, studentInfo.c_str(), studentInfo.length(), 0, (SOCKADDR *)&svrAddr, svrAddrLen);
    if (iResult == SOCKET_ERROR)
    {
        printf("sendto failed with error: %d\n", WSAGetLastError());
        closesocket(sc);
        WSACleanup();
        return 1;
    }

    // Nhan du lieu tra ve tu server
    char buffer[1024];
    iResult = recvfrom(sc, buffer, 1024, 0, (SOCKADDR *)&svrAddr, &svrAddrLen);
    if (iResult == SOCKET_ERROR)
    {
        cout<<"recvfrom failed with error: %d\n", WSAGetLastError();
        closesocket(sc);
        WSACleanup();
        return 1;
    }

    // In ra du lieu nhan duoc tu server
    buffer[iResult] = '\0';
    cout<<"Success: "<<endl;

    closesocket(sc);
    WSACleanup();
    getchar();
    return 0;
}
