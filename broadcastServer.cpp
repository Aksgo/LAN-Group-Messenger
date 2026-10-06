#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
int main(){
    SOCKET broadcastSocket;
    int port = 60000;
    //loading dll file to create sockets
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if(wsaerr!=0){
        cout<<"Winsock dll not found"<<endl;
        return 0;
    }else{
        cout<<"Winsock dll found"<<endl;
        cout<<"The status : "<<wsaData.szSystemStatus<<endl;
    }

    broadcastSocket = INVALID_SOCKET;
    broadcastSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockaddr_in baddr;
    baddr.sin_family = AF_INET;
    baddr.sin_port = htons(port);
    //please change when wifi network is changing
    //to other users : enter your own LAN's broadcast ipV4 address here
    const char* BROADCAST_IP = "10.98.66.255";
    baddr.sin_addr.s_addr = inet_addr(BROADCAST_IP);
    BOOL broadcastEnable = TRUE;
    setsockopt(broadcastSocket, SOL_SOCKET, SO_BROADCAST,(char*)&broadcastEnable, sizeof(broadcastEnable));
    const char* msg = "SERVER:55555";
    while(true){
        sendto(broadcastSocket,(const char*)msg, strlen(msg),0,(struct sockaddr*)&baddr, sizeof(baddr));
        cout<<"shouting...\n";
        Sleep(2000);
    }

    closesocket(broadcastSocket);
    WSACleanup();
    return 0;
}