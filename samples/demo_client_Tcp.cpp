#include <iostream>
#include <array>
#include "../asyncbsdsocket/include/tcp_client.h"
#include <string.h>
AsyncBsdSocketLib::TcpClient* client;
uint8_t sending(){
    const::std::size_t bufferSize = 5;
    std::array<uint8_t,bufferSize> buffer;
    char buff[256];
    bzero(buff,256);
    fgets(buff,255,stdin);
    buffer.fill(*buff);
    bool _sent = client->Send(buffer);
    if(_sent){
        std::cout << "The client sent character " << "'" << buffer.front() << "'" << " to the server\n";
    }
    return buffer.front();
}
int main(){
    const std::string cIpAddress = "127.0.0.1";
    const uint16_t port = 9900;
    client = new AsyncBsdSocketLib::TcpClient(cIpAddress,port);
    uint8_t send_data;
    bool _succeed = client->TrySetup();
    if(_succeed){
        std::cout << "client socket setup succeeded.\n";
       bool _succeed =  client->TryConnect();
       if(_succeed){
            std::cout << "client socket connection to server succeeded.\n";
            do{
                send_data = sending();
            }while(send_data != '#');
            std::cout << "Connection Terminated\n";
       }
    }
}