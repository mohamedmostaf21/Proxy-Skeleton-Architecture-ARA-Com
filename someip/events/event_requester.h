#ifndef EVENT_CLIENT
#define EVENT_CLIENT

#include "../../helper/concurrent_queue.h"
#include "../../helper/state.h"
#include "../someipRpcMsg/someip_rpc_message.h"
#include <future>
#include <functional>
#include <condition_variable>

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace pubsub
            {
                /// @brief SOME/IP event requester
                class EventSubscripter
                {
                public:   
                    using ReceivingHandler = std::function<void(const std::vector<uint8_t> &)>;
                
                private:
                    /******************************* attributes ******************************/
                    uint16_t mServiceId;
                    uint16_t mInstanceId;
                    uint8_t mMajorVersion;
                    uint8_t mMinorVersion;
                    uint16_t mEventgroupId;
                    uint8_t mProtocolVersion;
                    uint8_t mInterfaceVersion;
                    uint8_t mCounter;

                    uint16_t cInitialSessionId{1};
                    
                    const  uint16_t cRequestSetValueBySubscriberMethodId = 10;
                    const  uint16_t cRequestGetValueBySubscriberMethodId = 11;

                    helper::SubscriptionState state;

                    ReceivingHandler myReceivingHandle;

                    /******************************* useful variables ************************/

                    // queue at which we get received messages that contained acknowledging entry
                    helper::ConcurrentQueue<rpc::SomeIpRpcMessage> mMessageBuffer;

                    // mutex object
                    std::mutex mSubscriptionMutex;
                    // trier to take mutex
                    std::unique_lock<std::mutex> mSubscriptionLock;
                    std::condition_variable mSubscriptionConditionVariable;
                    // flag
                    bool mValidNotify;

                    /************************************* internal functions *****************/

                    bool isValidNotification(const rpc::SomeIpRpcMessage &request);
                   
                    bool isSubscriptionAck(const rpc::SomeIpRpcMessage &request);

                protected:
                    /*********************** useful for constructor of my child *****************/
                    
                    /// @param networkLayer Network communication abstraction layer
                    /// @param counter Counter to make the client distinguishable among other subscribers
                    EventSubscripter(
                            uint16_t serviceId,
                            uint16_t instanceId,
                            uint8_t majorVersion,
                            uint8_t minorVersion,
                            uint16_t eventgroupId,
                            uint8_t counter,
                            uint8_t protocolVersion,
                            uint8_t interfaceVersion = 1);
                    
                    /****************** function that child will implement it *****************/

                    virtual void Send(const rpc::SomeIpRpcMessage &message) = 0;

                    /******************** function take any someipRpc message *****************/

                    void InvokeEventHandler(rpc::SomeIpRpcMessage &&message);

                public:
                    /******************************* fundemental functions *********************/

                    void Subscribe(size_t maxSampleCount);
                    
                    helper::SubscriptionState GetSubscriptionState() const;

                    std::future<bool> getter(std::vector<uint8_t> &data); 

                    void printCurrentState() const;

                    void SetReceiveHandler(ReceivingHandler h);

                    void UnsetReceiveHandler();

                    /************************ disable empty constructor **********************/

                    EventSubscripter() = delete;
    
                    /******************************* destructor ******************************/

                    virtual ~EventSubscripter();
                };
            }
        }
    }
}

#endif