#include "CampaignStateProxy.h"

static const std::string c_AnyIpAddress{"0.0.0.0"};
static const std::string c_NicIpAddress{"127.0.0.1"};
static const std::string c_MulticastGroup{"239.0.0.1"};
static const uint16_t c_ServiceDiscoveryFindingPort{6666};
static const uint16_t c_ServiceId = 3;
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
                CampaignStateProxy::HandleType::HandleType(InstanceIdentifier id,int16_t p): identifier{id},
                                                                                            port_no{p}
                {}

                void CampaignStateProxy::HandleType::setId(InstanceIdentifier id)
                {   identifier=id;  }

                void CampaignStateProxy::HandleType::setPort(int16_t p)
                {   port_no=p;  }

                int16_t CampaignStateProxy::HandleType::getPort()const
                {   return port_no; }

                const InstanceIdentifier& CampaignStateProxy::HandleType::GetInstanceId()const
                {   return identifier;  }

                void CampaignStateProxy::HandleType::setRequester(FieldServiceRequester *r)
                {   requester = r;  }

                FieldServiceRequester* CampaignStateProxy::HandleType::getRequester()
                { return requester; }  


                ServiceHandleContainer<CampaignStateProxy::HandleType> CampaignStateProxy::findSerivce(InstanceIdentifier id)
                {
                    FieldServiceRequester *requester;
                    requester = new FieldServiceRequester(c_ServiceId,
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

                CampaignStateProxy::CampaignStateProxy (HandleType &handle)
                {
                    requester = handle.getRequester();
                }


                /************************ fundemental funtions *******************/

                void CampaignStateProxy::subscribe(size_t maxSampleCount)
                {
                    requester->fieldClient->Subscribe(maxSampleCount);
                }

                helper::SubscriptionState CampaignStateProxy::GetSubscriptionState() const
                {
                    return requester->fieldClient->GetSubscriptionState();
                }

                std::future<bool> CampaignStateProxy::set(std::vector<uint8_t> &data)
                {
                    return requester->fieldClient->set(data);
                }

                std::future<bool> CampaignStateProxy::get(std::vector<uint8_t> &data)
                {
                    return requester->fieldClient->get(data);
                }

                void CampaignStateProxy::printSubscriptionState()
                {
                    requester->fieldClient->printCurrentState();
                }

                void CampaignStateProxy::SetReceiveHandler(Handler h)
                {
                    requester->fieldClient->SetReceiveHandler(h);
                }

                void CampaignStateProxy::UnsetReceiveHandler()
                {
                    requester->fieldClient->UnsetReceiveHandler();
                }


                /********************* deconstructor *******************/
                
                CampaignStateProxy::~CampaignStateProxy()
                {
                    delete requester;
                }
        }
    }

}