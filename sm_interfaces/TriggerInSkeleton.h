#ifndef TRIGGER_IN_SKELETON_H
#define TRIGGER_IN_SKELETON_H

#include "../someip/service/field_Only/field_service_provider.h"
#include "../helper/instance_id.h"

namespace ara 
{
    namespace com
    {
        namespace skelton
        {
            class TriggerInSkelton : public ara::com::someip::sd::FieldServiceProvider 
            {                    
                public:
                    /****************** constructor ************************************/

                    TriggerInSkelton( ara::com::InstanceIdentifier id);  
                    

                    /*
                    init
                    offerSerivce
                    update
                    getState
                    */


                    /******** disable copy constructor and equal assigment operator *****/
      
                    TriggerInSkelton(const TriggerInSkelton &other) = delete;
                    TriggerInSkelton & operator=(const TriggerInSkelton &other) = delete;
                    
      
                    /********************* deconstructor *******************/

                    ~TriggerInSkelton();
            };
        }
    }
}

#endif