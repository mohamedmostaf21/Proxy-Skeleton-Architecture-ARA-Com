#ifndef TRIGGER_OUT_SKELETON_H
#define TRIGGER_OUT_SKELETON_H

#include "../someip/service/field_Only/field_service_provider.h"
#include "../helper/instance_id.h"

namespace ara 
{
    namespace com
    {
        namespace skelton
        {
            class TriggerOutSkelton : public ara::com::someip::sd::FieldServiceProvider 
            {                    
                public:
                    TriggerOutSkelton( ara::com::InstanceIdentifier id);  
                    
                    /*
                    init
                    offerSerivce
                    update
                    getState
                    */


                    /******** disable copy constructor and equal assigment operator *****/
      
                    TriggerOutSkelton(const TriggerOutSkelton &other) = delete;
                    TriggerOutSkelton & operator=(const TriggerOutSkelton &other) = delete;
                    
      
                    /********************* deconstructor *******************/

                    ~TriggerOutSkelton();
            };
        }
    }
}

#endif