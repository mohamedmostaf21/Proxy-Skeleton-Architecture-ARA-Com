#ifndef TRANSFER_STATE_SKELETON_H
#define TRANSFER_STATE_SKELETON_H

#include "../someip/service/field_Only/field_service_provider.h"
#include "../helper/instance_id.h"

namespace ara 
{
    namespace com
    {
        namespace skelton
        {
            class TransferStateSkeleton : public ara::com::someip::sd::FieldServiceProvider 
            {                    
                public:
                    /****************** constructor ************************************/

                    TransferStateSkeleton( ara::com::InstanceIdentifier id);
                    
                    /*
                    init
                    offerSerivce
                    update
                    getState
                    */

                    /******** disable copy constructor and equal assigment operator *****/
      
                    TransferStateSkeleton(const TransferStateSkeleton &other) = delete;
                    TransferStateSkeleton & operator=(const TransferStateSkeleton &other) = delete;
                    
      
                    /********************* deconstructor *******************/

                    ~TransferStateSkeleton();
            };
        }
    }
}

#endif