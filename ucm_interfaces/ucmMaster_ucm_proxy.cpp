#include "ucmMaster_ucm_proxy.h"

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
            /******************** constructors ************************/   
            UCMMASTER_UCM_Proxy::HandleType::HandleType(InstanceIdentifier id,int16_t p): identifier{id},
                                                                    port_no{p}
            {}


            /************ setters and getters *************************/

            void UCMMASTER_UCM_Proxy::HandleType::setId(InstanceIdentifier id)
            {   identifier=id;  }

            void UCMMASTER_UCM_Proxy::HandleType::setPort(int16_t p)
            {   port_no=p;  }

            int16_t UCMMASTER_UCM_Proxy::HandleType::getPort()const
            {   return port_no; }

            const InstanceIdentifier& UCMMASTER_UCM_Proxy::HandleType::GetInstanceId()const
            {   return identifier;  }

            void UCMMASTER_UCM_Proxy::HandleType::setRequester(UCM_Master_RPCSServiceRequester *r)
            {   requester = r;  }

            UCM_Master_RPCSServiceRequester* UCMMASTER_UCM_Proxy::HandleType::getRequester()
            { return requester; }    

            ServiceHandleContainer<UCMMASTER_UCM_Proxy::HandleType> UCMMASTER_UCM_Proxy::findSerivce(InstanceIdentifier id)
            {
                UCM_Master_RPCSServiceRequester *requester;
                requester = new UCM_Master_RPCSServiceRequester(c_ServiceId,
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
            UCMMASTER_UCM_Proxy::UCMMASTER_UCM_Proxy (HandleType &handle)
            {
                requester = handle.getRequester();
            }


            /************************ fundemental funtions *******************/
            std::future<bool> UCMMASTER_UCM_Proxy::RequestUpdateSession(std::vector<uint8_t> &data)
            {
                std::vector<uint8_t> payload;
                return requester->RequestUpdateSession(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::Activate(std::vector<uint8_t> &data)
            {
                std::vector<uint8_t> payload;
                return requester->Activate(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::finish(std::vector<uint8_t> &data)
            {
                std::vector<uint8_t> payload;
                return requester->Finish(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::GetId(std::vector<uint8_t> &data)
            {
                std::vector<uint8_t> payload;
                return requester->GetId(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::GetSwClusterChangeInfo(std::vector<uint8_t> &data)
            {
                std::vector<uint8_t> payload;
                return requester->GetSwClusterChangeInfo(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::GetSwPackages(std::vector<uint8_t> &data)
            {
                std::vector<uint8_t> payload;
                return requester->GetSwPackages(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::GetSwClusterInfo(std::vector<uint8_t> &data)
            {
                std::vector<uint8_t> payload;
                return requester->GetSwClusterInfo(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::RevertProcessedSwPackages(std::vector<uint8_t> &data)
            {
                std::vector<uint8_t> payload;
                return requester->RevertProcessedSwPackages(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::Rollback(std::vector<uint8_t> &data)
            {
                std::vector<uint8_t> payload;
                return requester->Rollback(payload,data);
            }


            std::future<bool> UCMMASTER_UCM_Proxy::PrepareUpdate(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->PrepareUpdate(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::VerifyUpdate(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->VerifyUpdate(payload,data);
            }
            
            std::future<bool> UCMMASTER_UCM_Proxy::PrepareRollback(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->PrepareRollback(payload,data);
            }
           
            std::future<bool> UCMMASTER_UCM_Proxy::cancell(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->cancel(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::DeleteTransfer(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->DeleteTransfer(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::GetHistory(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->GetHistory(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::GetSwClusterManifestInfo(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->GetSwClusterManifestInfo(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::GetSwProcessProgress(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->GetSwProcessProgress(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::ProcessSwPackage(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->ProcessSwPackage(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::TransferData(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->TransferData(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::TransferExit(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->TransferExit(payload,data);
            }

            std::future<bool> UCMMASTER_UCM_Proxy::TransferStart(const std::vector<uint8_t> &payload,
                            std::vector<uint8_t> &data)
            {
                return requester->TransferStart(payload,data);
            }


            void UCMMASTER_UCM_Proxy::StopUpdateSession()
            {
                requester->StopUpdateSession();
            }

            void UCMMASTER_UCM_Proxy::ResetMachine()
            {
                requester->ResetMachine();
            }


            /********************* deconstructor *******************/
            UCMMASTER_UCM_Proxy::~UCMMASTER_UCM_Proxy()
            {
                    delete requester;
            }
        }
    }

}