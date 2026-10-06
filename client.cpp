#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <sstream>
#include <string>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(){
    SOCKET clientSocket, ipSocket;
    int port = 55554;
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

    //receiver socket for udp broadcast server
    ipSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    sockaddr_in rec_addr;
    rec_addr.sin_family = AF_INET;
    rec_addr.sin_port = htons(60000);
    rec_addr.sin_addr.s_addr = INADDR_ANY;
    char udpBuffer[512];
    bind(ipSocket, (sockaddr*)&rec_addr, sizeof(rec_addr));
    sockaddr_in clientService;
    int len = sizeof(clientService);
    int bytes = recvfrom(ipSocket, udpBuffer, sizeof(udpBuffer)-1, 0, (sockaddr*)&clientService, &len);
    if (bytes > 0) {
        udpBuffer[bytes] = '\0';
        string sender;
        string server_port;
        stringstream s(udpBuffer);
        getline(s,sender,':');
        s>>server_port;
        cout<<server_port<<endl;
        port = stoi(server_port);
        cout << "Found server at: " << inet_ntoa(clientService.sin_addr) << endl;
        cout<<"Port : "<<port<<endl;
    }

    //client socket setup
    clientSocket = INVALID_SOCKET;
    clientSocket = socket(2,1,6);
    if(clientSocket == INVALID_SOCKET){
        cout<<"Error at socket() :"<<WSAGetLastError()<<endl;
        WSACleanup();
        return 0;
    }else{
        cout<<"Socket is OK()"<<endl;
    }

    //connect to server
    clientService.sin_family = AF_INET;
    //clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(port);
    if(connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService))==SOCKET_ERROR){
        cout<<"Client: conenct() - failed to connect :"<<WSAGetLastError()<<endl;
        WSACleanup();
        return 0;
    }else{
        cout<<"Client: connect() OK"<<endl;
        cout<<"Client: --- Can transfer data --- "<<endl;
    }

    //chat time
    bool state = true;
    while(state){
        char buffer[200];
        cout<<"Please enter you message to server : ";
        cin.getline(buffer,200);
        int byte_count = send(clientSocket,buffer,200,0);
        if(byte_count>0){
        }else{
            WSACleanup();
        }
        if(strcmp("shut",buffer)==0){state = false;continue;}
        byte_count = recv(clientSocket,buffer,200,0);
        if(byte_count>0){
            cout<<"Message received : "<<buffer<<endl;
        }else{
            WSACleanup();
        }
        if(strcmp("shut",buffer)==0){state = false;}
    }
    closesocket(clientSocket);
    system("pause");
    WSACleanup();
    return 0;
}