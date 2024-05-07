#ifndef EVENT_SERVER
#define EVENT_SERVER

#include "../../helper/fsm.h"
#include "fsm/service_down_state.h"
#include "fsm/notsubscribed_state.h"
#include "fsm/subscribed_state.h"
#include "../someipRpcMsg/someip_rpc_message.h"

#include <functional>
#include <stdint.h>
#include <vector>


namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace pubsub
            {
                /// @brief SOME/IP event server
                class EventServer
                {
                public:
                    using ProcessingHandler = std::function<bool(const std::vector<uint8_t> &)>;

                private:
                    /******************************* attributes ******************************/

                    uint16_t mServiceId;
                    uint16_t mInstanceId;
                    uint8_t mMajorVersion;
                    uint16_t mEventgroupId;
                    const uint8_t mProtocolVersion;
                    const uint8_t mInterfaceVersion;                    
                    uint16_t cInitialSessionId{1};

                    helper::FSM<helper::PubSubState> mStateMachine;
                    fsm::ServiceDownState mServiceDownState;
                    fsm::NotSubscribedState mNotSubscribedState;
                    fsm::SubscribedState mSubscribedState;

                    ProcessingHandler myProcessHandle;
                    

                    /*************************** internal function **********************/
                    
                    bool isRequestingToSubscription(const rpc::SomeIpRpcMessage &request);
                                      
                    void printCurrentState() const;


                protected:
                    /****************** function that child will implement it *****************/

                    virtual void Send(const std::vector<uint8_t> &payload) = 0;


                    /*********** function contain what to do with received messages ***********/

                    void InvokeEventHandler(const rpc::SomeIpRpcMessage &request);


                public:
                    /******************************* constructor ******************************/

                    /// @param networkLayer Network communication abstraction layer
                    /// @param serviceId Service ID
                    /// @param instanceId Service instance ID
                    /// @param majorVersion Service major version
                    /// @param eventgroupId Service event-group ID
                    /// @param ipAddress Multicast IP address that clients should listen to for receiving events
                    /// @param port Multicast port number that clients should listen to for receiving events
                    EventServer( uint16_t serviceId,
                                 uint16_t instanceId,
                                 uint8_t majorVersion,
                                 uint16_t eventgroupId,
                                 uint8_t protocolVersion,
                                 uint8_t interfaceVersion);

                    
                    /******************************* fundemental functions *********************/

                    /// @brief Start the server
                    void Start();

                    /// @brief Stop the server
                    void Stop();

                    /// @brief Get the current server state
                    /// @returns Server machine state
                    helper::PubSubState GetState() const noexcept;

                    void update(const std::vector<uint8_t> &data);
                    
                    void SetHandler(ProcessingHandler handler);


                    /******************************* destructor ******************************/

                    virtual ~EventServer();
                };
            }
        }
    }
}

#endif