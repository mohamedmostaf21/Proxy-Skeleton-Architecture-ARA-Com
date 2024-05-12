#ifndef PROVIDER_H
#define PROVIDER_H

#include "../../../sockets/include/poller.h"
#include "../../../sockets/include/udp_client.h"
#include "../../../helper/concurrent_queue.h"
#include "../../../helper/ipv4_address.h"
#include "../../someipSdMsg/someip_sd_message.h"
#include "../../someipSdMsg/entry/service_entry.h"
#include "../../someipSdMsg/entry/eventgroup_entry.h"
#include "../../someipSdMsg/option/ipv4_endpoint_option.h"
#include "../../rpc/socket_rpc_server.h"
#include "../../events/socket_event_server.h"
#include <algorithm>
#include <iostream>
#include "../../../config.h"


namespace ara
{
    namespace com
    {
        namespace someip
        {
            /// @brief SOME/IP service discovery namespace
            /// @note The namespace is not part of the ARA standard.
            namespace sd
            {

#if(EXAMPLE == RPCS)
                using HandlerTypeFunc = std::function<bool(const std::vector<uint8_t> &, std::vector<uint8_t> &)>;
                const uint16_t cSumationOverVectorMethodId = 1000;
                const uint16_t cMultiplicationOverVectorMethodID = 2000;
                const uint16_t cGetSumMethodID = 3000;

                const uint16_t  cRequestUpdateSessionMethodID  = 4001;
      
                const uint16_t  cPrepareUpdateMethodID  = 4002;
                const uint16_t  cVerifyUpdateMethodID  = 4003;
                const uint16_t  cPrepareRollbackMethodID  = 4004;

                const uint16_t cStopUpdateSessionMethodID = 4005;
                const uint16_t cResetMachineMethodID = 4006;                
#elif(EXAMPLE == PUBSUB)

#endif

                class Provider
                {
                public:

#if(EXAMPLE == RPCS)
                   
                    rpc::SocketRpcServer *rpcServer;
                    
#elif(EXAMPLE == PUBSUB)
                    
                    SockeKEventServer *eventServer;
#endif                    
                
                private:
                    /******************* attributes ********************************/

                    uint16_t mServiceId;
                    uint16_t mInstanceId;
                    uint8_t mMajorVersion;
                    uint8_t mMinorVersion;
                    uint16_t mEventgroupId;
                    //helper::Ipv4Address mEndpointIp;
                    uint16_t mEndpointRpcsPort;
                    uint16_t mEndpointEventPort;

                    const uint8_t mProtocolVersion;
                    const uint8_t mInterfaceVersion;

                    /*********************** poller attributes  ********************/

                    static const size_t cBufferSize{256};
                    const std::string cAnyIpAddress = "0.0.0.0";
                    helper::ConcurrentQueue<std::vector<uint8_t>> mSendingQueue;
                    AsyncBsdSocketLib::Poller *const mPoller;
                    AsyncBsdSocketLib::UdpClient mUdpSocket;

                    const std::string cNicIpAddress;
                    const std::string cMulticastGroup;
                    const uint16_t cPort;
                                 
                    /********************** poller functions  *********************/

                    void onReceive();
                    void onSend();
                    void Send(const SomeIpSdMessage &message);   

                public:
                    /******************* constructor  *******************************/

                    /// @brief Constructor
                    /// @param poller BSD sockets poller
                    /// @param nicIpAddress Network interface controller IPv4 address
                    /// @param multicastGroup Multicast group IPv4 address
                    /// @param port Multicast UDP port number
                    /// @throws std::runtime_error Throws when the UDP socket configuration failed
                    Provider(
                        uint16_t serviceId,
                        uint16_t instanceId,
                        uint8_t majorVersion,
                        uint8_t minorVersion,
                        uint16_t eventgroupId, 
                        AsyncBsdSocketLib::Poller *poller,
                        std::string nicIpAddress,
                        std::string multicastGroup,
                        uint16_t port,
                        uint16_t endpointRpcsPort,
                        uint16_t endpointEventPort,
                        uint8_t protocolVersion,
                        uint8_t interfaceVersion = 1);


                    /************************ fundemental functions *********************/

                    void offerService();

                    void stopService();
                    
                    void init();

                    AsyncBsdSocketLib::Poller* getPoller();


                    /******************* abstract functions *****************************/

                    bool RequestUpdateSessionImp()
                    {                        
                        return false;
                    }

                    bool RequestUpdateSession(const std::vector<uint8_t> &input, std::vector<uint8_t> &output) 
                    {
                        std::cout << "\nRequestUpdateSession is called\n";

                        bool funcResult = RequestUpdateSessionImp();

                        output.push_back(funcResult); // Put the sum in the output vector

                        return true;
                    }

                    bool PrepareUpdateImp(const std::vector<uint8_t> &list)
                    {
                        for (const auto &element : list) 
                        {
                            std::cout << static_cast<int>(element) << " "; 
                        } 
                        std::cout << std::endl;
                        
                        return true;
                    }

                    bool PrepareUpdate(const std::vector<uint8_t> &input, std::vector<uint8_t> &output) 
                    {
                        std::cout << "\nPrepareUpdate is called\n";

                        bool funcResult = PrepareUpdateImp(input);

                        output.push_back(funcResult); // Put the sum in the output vector

                        return true;
                    }

                    bool VerifyUpdateImp(const std::vector<uint8_t> &list)
                    {
                        for (const auto &element : list) 
                        {
                            std::cout << static_cast<int>(element) << " "; 
                        } 
                        std::cout << std::endl;
                        
                        return true;
                    }

                    bool VerifyUpdate(const std::vector<uint8_t> &input, std::vector<uint8_t> &output) 
                    {
                        std::cout << "\nVerifyUpdate is called\n";

                        bool funcResult = VerifyUpdateImp(input);

                        output.push_back(funcResult); // Put the sum in the output vector

                        return true;
                    }

                    bool PrepareRollbackImp(const std::vector<uint8_t> &list)
                    {
                        for (const auto &element : list) 
                        {
                            std::cout << static_cast<int>(element) << " "; 
                        } 
                        std::cout << std::endl;
                        
                        return true;
                    }

                    bool PrepareRollback(const std::vector<uint8_t> &input, std::vector<uint8_t> &output) 
                    {
                        std::cout << "\nPrepareRollback is called\n";

                        bool funcResult = PrepareRollbackImp(input);

                        output.push_back(funcResult); // Put the sum in the output vector

                        return true;
                    }



                    virtual void StopUpdateSessionImp()
                    {
                        std::cout << "StopUpdateSessionImp from provider\n";
                    }
                
                    bool StopUpdateSession(const std::vector<uint8_t> &input, std::vector<uint8_t> &output)
                    {
                        StopUpdateSessionImp();
                        return false;
                    }

                    virtual void ResetMachineImp()
                    {
                        std::cout << "ResetMachine from provider\n";
                    }
                
                    bool ResetMachine(const std::vector<uint8_t> &input, std::vector<uint8_t> &output)
                    {
                        ResetMachineImp();
                        return false;
                    }
                    

                    /******************************  deconstructor  *********************/
                    ~Provider();   
                };
            }
        }
    }
}

#endif