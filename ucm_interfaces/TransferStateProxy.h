#ifndef TRANSFER_STATE_PROXY_H
#define TRANSFER_STATE_PROXY_H

#include "../someip/service/field_Only/field_service_requester.h"
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

            class TransferStateProxy
            {
                public:
                    using Handler = std::function<void(const std::vector<uint8_t> &)>;

                    class HandleType
                    {
                        private:  
                            /********************** attributes ************************/
                         
                            ara::com::InstanceIdentifier identifier;
                            int16_t port_no;
                            FieldServiceRequester *requester;

                        
                        public:
                            /******************** constructors ************************/
                        
                            HandleType(InstanceIdentifier id,int16_t p);


                            /************ setters and getters *************************/

                            void setId(InstanceIdentifier id);

                            void setPort(int16_t p);

                            int16_t getPort()const;

                            const InstanceIdentifier &GetInstanceId()const;

                            void setRequester(FieldServiceRequester *r);

                            FieldServiceRequester* getRequester();  
                    };


                    /*********************** static functions **********************/

                    static ServiceHandleContainer<HandleType> findSerivce(InstanceIdentifier id);


                    /************************* constructor **************************/

                    TransferStateProxy (HandleType &handle);
   

                    /************************ fundemental funtions *******************/

                    void subscribe(size_t maxSampleCount);
 
                    helper::SubscriptionState GetSubscriptionState() const;

                    std::future<bool> set(std::vector<uint8_t> &data);

                    std::future<bool> get(std::vector<uint8_t> &data);

                    void printSubscriptionState();
                    
                    void SetReceiveHandler(Handler h);

                    void UnsetReceiveHandler();


                    /******** disable copy constructor and equal assigment operator *****/
                    
                    TransferStateProxy(const TransferStateProxy &other) = delete;
                    TransferStateProxy& operator=(const TransferStateProxy &other) = delete;


                    /********************* deconstructor *******************/
                    
                    ~TransferStateProxy();

                private:
                    /*************************** atttibutes ****************/

                    FieldServiceRequester *requester;
            };
        }
    }
}

#endif