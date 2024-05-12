#ifndef CAMPAIGN_STATE_SKELETON_H
#define CAMPAIGN_STATE_SKELETON_H

#include "../someip/service/field_Only/field_service_provider.h"
#include "../helper/instance_id.h"

namespace ara 
{
    namespace com
    {
        namespace skelton
        {
            class CampaignStateSkeleton : public ara::com::someip::sd::FieldServiceProvider 
            {                    
                public:
                    /****************** constructor ************************************/

                    CampaignStateSkeleton( ara::com::InstanceIdentifier id);
                    
                    /*
                    init
                    offerSerivce
                    update
                    getState
                    */

                    /******** disable copy constructor and equal assigment operator *****/
      
                    CampaignStateSkeleton(const CampaignStateSkeleton &other) = delete;
                    CampaignStateSkeleton & operator=(const CampaignStateSkeleton &other) = delete;
                    
      
                    /********************* deconstructor *******************/

                    ~CampaignStateSkeleton();
            };
        }
    }
}

#endif