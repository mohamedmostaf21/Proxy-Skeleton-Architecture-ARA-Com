#ifndef UCM_MASTER_UCM_PROXY_H
#define UCM_MASTER_UCM_PROXY_H

#include "../someip/service/rpcs_Only/ucm_master_rpcs_service_requester.h"
#include "../helper/instance_id.h"

using namespace ara::com::someip::sd;
using namespace ara::com::helper;
using namespace AsyncBsdSocketLib;

namespace ara 
{
    namespace com
    {
        namespace proxy
        {    
            template <typename t>
            using ServiceHandleContainer = std::vector<t>;

            class UCMMASTER_UCM_Proxy
            {
                public:
                    class HandleType
                    {
                        private:  
                            /********************** attributes ************************/                     
                            ara::com::InstanceIdentifier identifier;
                            int16_t port_no;
                            UCM_Master_RPCSServiceRequester *requester;

                        
                        public:
                            /******************** constructor ************************/                  
                            HandleType(InstanceIdentifier id,int16_t p);


                            /************ setters and getters *************************/
                            void setId(InstanceIdentifier id);

                            void setPort(int16_t p);

                            int16_t getPort()const;

                            const InstanceIdentifier &GetInstanceId()const;

                            void setRequester(UCM_Master_RPCSServiceRequester *r);

                            UCM_Master_RPCSServiceRequester* getRequester();  
                    };


                    /*********************** static functions **********************/
                    static ServiceHandleContainer<HandleType> findSerivce(InstanceIdentifier id);


                    /************************* constructor **************************/
                    UCMMASTER_UCM_Proxy (HandleType &handle);
   

                    /************************ fundemental funtions *******************/
                    std::future<bool> RequestUpdateSession(std::vector<uint8_t> &data);                                

                    std::future<bool> Activate(std::vector<uint8_t> &data);     
                    std::future<bool> finish(std::vector<uint8_t> &data);   
                    std::future<bool> GetId(std::vector<uint8_t> &data);   
                    std::future<bool> GetSwClusterChangeInfo(std::vector<uint8_t> &data);   
                    std::future<bool> GetSwPackages(std::vector<uint8_t> &data);   
                    std::future<bool> GetSwClusterInfo(std::vector<uint8_t> &data);   
                    std::future<bool> RevertProcessedSwPackages(std::vector<uint8_t> &data);   
                    std::future<bool> Rollback(std::vector<uint8_t> &data);   




                    std::future<bool> PrepareUpdate(const std::vector<uint8_t> &payload, std::vector<uint8_t> &data);
                    std::future<bool> VerifyUpdate(const std::vector<uint8_t> &payload, std::vector<uint8_t> &data);
                    std::future<bool> PrepareRollback(const std::vector<uint8_t> &payload, std::vector<uint8_t> &data);

                    std::future<bool> cancell(const std::vector<uint8_t> &payload, std::vector<uint8_t> &data);
                    std::future<bool> DeleteTransfer(const std::vector<uint8_t> &payload, std::vector<uint8_t> &data);
                    std::future<bool> GetHistory(const std::vector<uint8_t> &payload, std::vector<uint8_t> &data);
                    std::future<bool> GetSwClusterManifestInfo(const std::vector<uint8_t> &payload, std::vector<uint8_t> &data);
                    std::future<bool> GetSwProcessProgress(const std::vector<uint8_t> &payload, std::vector<uint8_t> &data);
                    std::future<bool> ProcessSwPackage(const std::vector<uint8_t> &payload, std::vector<uint8_t> &data);
                    std::future<bool> TransferData(const std::vector<uint8_t> &payload, std::vector<uint8_t> &data);
                    std::future<bool> TransferExit(const std::vector<uint8_t> &payload, std::vector<uint8_t> &data);
                    std::future<bool> TransferStart(const std::vector<uint8_t> &payload, std::vector<uint8_t> &data);




                    void StopUpdateSession();
                    void ResetMachine();


                    /******** disable copy constructor and equal assigment operator *****/ 
                    UCMMASTER_UCM_Proxy(const UCMMASTER_UCM_Proxy &other) = delete;
                    UCMMASTER_UCM_Proxy& operator=(const UCMMASTER_UCM_Proxy &other) = delete;


                    /********************* deconstructor *******************/          
                    ~UCMMASTER_UCM_Proxy();


                private:
                    /*************************** atttibutes ****************/
                    HandleType  *Handle;
                    UCM_Master_RPCSServiceRequester *requester;
            };
        }
    }

}

#endif