#ifndef PACKAGE_MANAGEMENT_SKELETON_H
#define PACKAGE_MANAGEMENT_SKELETON_H

#include "../someip/service/field_Only/field_service_provider.h"
#include "../helper/instance_id.h"

namespace ara 
{
    namespace com
    {
        namespace skelton
        {
            class PackageManagementSkelton : public ara::com::someip::sd::FieldServiceProvider 
            {                    
                public:
                    /****************** constructor ************************************/

                    PackageManagementSkelton( ara::com::InstanceIdentifier id);
                    
                    /*
                    init
                    offerSerivce
                    update
                    getState
                    */

                    /******** disable copy constructor and equal assigment operator *****/
      
                    PackageManagementSkelton(const PackageManagementSkelton &other) = delete;
                    PackageManagementSkelton & operator=(const PackageManagementSkelton &other) = delete;
                    
      
                    /********************* deconstructor *******************/

                    ~PackageManagementSkelton();
            };
        }
    }
}

#endif