#include "Event_DENM_Skeleton.h"   

static const std::string c_AnyIpAddress{"0.0.0.0"};
static const std::string c_NicIpAddress{"127.0.0.1"};
static const std::string c_MulticastGroup{"239.0.0.1"};
static const uint16_t c_ServiceDiscoveryOfferingPort{5555};
static const uint16_t c_Udp_port = 1800;
static const uint16_t c_ServiceId = 2;
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
            Event_DENM_Skeleton::Event_DENM_Skeleton( ara::com::InstanceIdentifier id) : EventServiceProvider
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
                                                                    c_Udp_port,        
                                                                    c_ProtocolVersion
                                                                    )  
            {}  
            

            /********************* deconstructor *******************/

            Event_DENM_Skeleton::~Event_DENM_Skeleton()
            {

            }
        }
    }
}
