#ifndef SW_NAME_VERSION_SKELETON_H
#define SW_NAME_VERSION_SKELETON_H

#include "../someip/service/field_Only/field_service_provider.h"
#include "../helper/instance_id.h"

namespace ara 
{
    namespace com
    {
        namespace skelton
        {
            class SwNameVersionSkelton : public ara::com::someip::sd::FieldServiceProvider 
            {                    
                public:
                    /****************** constructor ************************************/

                    SwNameVersionSkelton( ara::com::InstanceIdentifier id);
                    
                    /*
                    init
                    offerSerivce
                    update
                    getState
                    */

                    /******** disable copy constructor and equal assigment operator *****/
      
                    SwNameVersionSkelton(const SwNameVersionSkelton &other) = delete;
                    SwNameVersionSkelton & operator=(const SwNameVersionSkelton &other) = delete;
                    
      
                    /********************* deconstructor *******************/

                    ~SwNameVersionSkelton();
            };
        }
    }
}

#endif