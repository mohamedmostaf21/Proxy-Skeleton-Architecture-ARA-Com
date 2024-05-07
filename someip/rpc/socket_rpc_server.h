#ifndef SOCKET_RPC_SERVER_H
#define SOCKET_RPC_SERVER_H

#include "../../sockets/include/poller.h"
#include "../../helper/concurrent_queue.h"
#include "rpcs_responser.h"
#include <algorithm>
#include <iostream>

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace rpc
            {
                /// @brief TCP socket-based RPC server
                class SocketRpcServer : public RpcsResponser
                {
                private:
                    /*********************** poller attributes  *******************/

                    static const size_t cBufferSize{256};
                    helper::ConcurrentQueue<std::vector<uint8_t>> mSendingQueue;
                    AsyncBsdSocketLib::Poller *const mPoller;
                    AsyncBsdSocketLib::TcpListener mServer;



                    /********************** poller functions  *********************/

                    void onAccept();
                    void onReceive();
                    void onSend();

                public:
                    /******************* constructor  *******************************/

                    /// @brief Constructor
                    /// @param poller BSD sockets poller
                    /// @param ipAddress RPC server IP address
                    /// @param port RPC server listening TCP port number
                    /// @param protocolVersion SOME/IP protocol header version
                    /// @param interfaceVersion Service interface version
                    /// @throws std::runtime_error Throws when the TCP server socket configuration failed
                    SocketRpcServer(
                        AsyncBsdSocketLib::Poller *poller,
                        std::string ipAddress,
                        uint16_t port,
                        uint8_t protocolVersion,
                        uint8_t interfaceVersion = 1);



                    /**************** override deconstructor  *********************/

                    virtual ~SocketRpcServer() override;
                };
            }
        }
    }
}

#endif