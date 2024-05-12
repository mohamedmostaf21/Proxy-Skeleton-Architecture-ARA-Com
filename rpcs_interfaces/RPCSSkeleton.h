#ifndef RPCS_SKELETON_H
#define RPCS_SKELETON_H

#include "../someip/service/rpcs_Only/rpcs_service_provider.h"
#include "../helper/instance_id.h"

namespace ara 
{
    namespace com
    {
        namespace skelton
        {
            class RPCSSkelton : public ara::com::someip::sd::RPCSServiceProvider 
            {                    
                public:
                    /***************** constructor ************************/                 
                    RPCSSkelton( ara::com::InstanceIdentifier id);
                    

                    /*
                    init
                    offerSerivce
                    */

                    /**************** override abstract functions ********/
                    virtual bool RequestUpdateSessionImp() override;

                    virtual bool PrepareUpdateImp(const std::vector<uint8_t> &list) override;

                    virtual bool VerifyUpdateImp(const std::vector<uint8_t> &list) override;
 
                    virtual bool PrepareRollbackImp(const std::vector<uint8_t> &list) override;

                    virtual void StopUpdateSessionImp() override;            

                    virtual void ResetMachineImp() override;     


                    /******** disable copy constructor and equal assigment operator *****/
                    RPCSSkelton(const RPCSSkelton &other) = delete;
                    RPCSSkelton & operator=(const RPCSSkelton &other) = delete;
                    
                    /********************* deconstructor *******************/
                    ~RPCSSkelton();
            };
            
        }
    }
}

#endif