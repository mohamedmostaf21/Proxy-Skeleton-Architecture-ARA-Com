#ifndef SOCKET_RPC_SERVER_H
#define SOCKET_RPC_SERVER_H

#include "../../asyncbsdsocket/include/poller.h"
#include "../../helper/concurrent_queue.h"
#include "./rpc_server.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace rpc
            {
                /// @brief TCP socket-based RPC server
                class SocketRpcServer : public RpcServer
                {
                private:
                    /******************************* attributes  *************************************/

                    static const size_t cBufferSize{256};

                    helper::ConcurrentQueue<std::vector<uint8_t>> mSendingQueue;
                    AsyncBsdSocketLib::Poller *const mPoller;
                    AsyncBsdSocketLib::TcpListener mServer;



                    /**************************** backend functions  **********************************/

                    void onAccept();
                    void onReceive();
                    void onSend();

                public:
                    /**************************** constructor  ****************************************/

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



                    /**************************** override deconstructor inherited from parent *********/

                    virtual ~SocketRpcServer() override;
                };
            }
        }
    }
}

#endif