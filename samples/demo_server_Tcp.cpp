#include <iostream>
#include <array>
#include "../asyncbsdsocket/include/tcp_listener.h"
#include <string.h>
AsyncBsdSocketLib::TcpListener* server;
uint8_t receiving(){
    const::std::size_t bufferSize = 5;
    std::array<uint8_t,bufferSize> buffer;
    bool _received = server->Receive(buffer);
    if(_received){
           std::cout << "The server received character " << "'" << buffer.front() << "'" << " from the client\n";
    }
    return buffer.front();
}
int main(){
    const std::string cIpAddress = "127.0.0.1";
    const uint16_t port = 9900;
    server = new AsyncBsdSocketLib::TcpListener(cIpAddress,port);
    uint8_t receive_data;
    bool _succeed = server->TrySetup();
    if(_succeed){
        std::cout << "server socket setup succeeded.\n";
        std::cout << "waiting for client connection...\n";
       bool _succeed =  server->TryAccept();
       if(_succeed){
            std::cout << "server socket accepted connection from a client.\n";
            do{
                receive_data = receiving();
            }while(receive_data != '#');
            std::cout << "Connection Terminated\n";
       }
    }
}