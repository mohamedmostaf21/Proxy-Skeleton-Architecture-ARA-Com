#include "RPCSSkeleton.h"


static const std::string c_AnyIpAddress{"0.0.0.0"};
static const std::string c_NicIpAddress{"127.0.0.1"};
static const std::string c_MulticastGroup{"239.0.0.1"};
static const uint16_t c_ServiceDiscoveryOfferingPort{5555};
static const uint16_t c_Tcp_port = 7500;
static const uint16_t c_ServiceId = 1;
static const uint8_t c_MajorVersion = 10;
static const uint8_t c_MinorVersion = 4;
static const uint16_t c_EventgroupId = 5;
static const uint8_t c_ProtocolVersion = 20;
static const uint16_t c_InterfaceVersion = 2;
static const uint16_t c_ClientId = 1;
static const int cTimeoutMs = 100;


namespace ara 
{
    namespace com
    {
        namespace skelton
        {
            /***************** constructor ************************/
            RPCSSkelton::RPCSSkelton( ara::com::InstanceIdentifier id) : RPCSServiceProvider
                                                                ( 
                                                                    c_ServiceId,        
                                                                    id.getInstanceId(),
                                                                    c_MajorVersion,                   
                                                                    c_MinorVersion,                       
                                                                    c_EventgroupId,                          
                                                                    new AsyncBsdSocketLib::Poller(),
                                                                    c_NicIpAddress,
                                                                    c_MulticastGroup,
                                                                    c_ServiceDiscoveryOfferingPort,
                                                                    c_Tcp_port,                     
                                                                    c_ProtocolVersion
                                                                )  
            {}  


            bool RPCSSkelton::RequestUpdateSessionImp()
            {                        
                return false;
            }

            bool RPCSSkelton::PrepareUpdateImp(const std::vector<uint8_t> &list)
            {
                for (const auto &element : list) 
                {
                    std::cout << static_cast<int>(element) << " "; 
                } 
                std::cout << std::endl;
                
                return true;
            }

            bool RPCSSkelton::VerifyUpdateImp(const std::vector<uint8_t> &list)
            {
                for (const auto &element : list) 
                {
                    std::cout << static_cast<int>(element) << " "; 
                } 
                std::cout << std::endl;
                
                return true;
            }

            bool RPCSSkelton::PrepareRollbackImp(const std::vector<uint8_t> &list) 
            {
                for (const auto &element : list) 
                {
                    std::cout << static_cast<int>(element) << " "; 
                } 
                std::cout << std::endl;
                
                return true;
            }

            void RPCSSkelton::StopUpdateSessionImp()
            {
                std::cout << "StopUpdateSessionImp from provider\n";
            }             

            void RPCSSkelton::ResetMachineImp()
            {
                std::cout << "ResetMachine from provider\n";
            }     


            /********************* deconstructor *******************/
            RPCSSkelton::~RPCSSkelton()
            {}
        }
    }
}
