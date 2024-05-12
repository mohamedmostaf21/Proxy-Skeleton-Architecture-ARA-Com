#include "RPCSProxy.h"

static const std::string c_AnyIpAddress{"0.0.0.0"};
static const std::string c_NicIpAddress{"127.0.0.1"};
static const std::string c_MulticastGroup{"239.0.0.1"};
static const uint16_t c_ServiceDiscoveryFindingPort{6666};
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
        namespace proxy
        {    
            /******************** constructors ************************/   
            RPCSProxy::HandleType::HandleType(InstanceIdentifier id,int16_t p): identifier{id},
                                                                    port_no{p}
            {}


            /************ setters and getters *************************/

            void RPCSProxy::HandleType::setId(InstanceIdentifier id)
            {   identifier=id;  }

            void RPCSProxy::HandleType::setPort(int16_t p)
            {   port_no=p;  }

            int16_t RPCSProxy::HandleType::getPort()const
            {   return port_no; }

            const InstanceIdentifier& RPCSProxy::HandleType::GetInstanceId()const
            {   return identifier;  }

            void RPCSProxy::HandleType::setRequester(RPCSServiceRequester *r)
            {   requester = r;  }

            RPCSServiceRequester* RPCSProxy::HandleType::getRequester()
            { return requester; }    

            ServiceHandleContainer<RPCSProxy::HandleType> RPCSProxy::findSerivce(InstanceIdentifier id)
            {
                RPCSServiceRequester *requester;
                requester = new RPCSServiceRequester(c_ServiceId,
                                        id.getInstanceId(),
                                        c_MajorVersion,
                                        c_MinorVersion,
                                        c_EventgroupId,
                                        new AsyncBsdSocketLib::Poller(),
                                        c_NicIpAddress,
                                        c_MulticastGroup,
                                        c_ServiceDiscoveryFindingPort,
                                        c_ProtocolVersion);
                
                Poller *poller = requester->getPoller();
                bool *running = new bool(true);
                std::thread t1([poller,running](){
                    while(*running)
                    {
                        poller->TryPoll(cTimeoutMs);
                    }
                });

                uint16_t _port;
                bool _result = requester->findService(_port);
                if(!_result)
                {
                    std::vector<HandleType> handles;
                    
                    *running = false;
                    //std::cout << "11111111111111\n";
                    t1.join();
                    delete running;
                    //std::cout << "11111111111111\n";
                    return handles;
                }
                else
                {
                    std::vector<HandleType> handles;
                    HandleType handleTypeObj(id,_port);
                    handleTypeObj.setRequester(requester);
                    handles.push_back(handleTypeObj);
                    
                    *running = false;
                    //std::cout << "2222222222222222222\n";
                    t1.join();
                    delete running;
                    //std::cout << "2222222222222222222\n";
                    return handles;
                }
            }

            /************************* constructor **************************/
            RPCSProxy::RPCSProxy (HandleType &handle)
            {
                requester = handle.getRequester();
            }


            /************************ fundemental funtions *******************/
            std::future<bool> RPCSProxy::RequestUpdateSession(std::vector<uint8_t> &data)
            {
                std::vector<uint8_t> payload;
                return requester->RequestUpdateSession(payload,data);
            }


            std::future<bool> RPCSProxy::PrepareUpdate(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->PrepareUpdate(payload,data);
            }

            std::future<bool> RPCSProxy::VerifyUpdate(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->VerifyUpdate(payload,data);
            }
            
            std::future<bool> RPCSProxy::PrepareRollback(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->PrepareRollback(payload,data);
            }
           

            void RPCSProxy::StopUpdateSession()
            {
                requester->StopUpdateSession();
            }

            void RPCSProxy::ResetMachine()
            {
                requester->ResetMachine();
            }


            /********************* deconstructor *******************/
            RPCSProxy::~RPCSProxy()
            {
                    delete requester;
            }
        }
    }

}