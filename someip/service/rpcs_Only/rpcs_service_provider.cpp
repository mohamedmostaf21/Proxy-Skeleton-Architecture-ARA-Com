#include "rpcs_service_provider.h"

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                /******************************* constants **********************************/
                using HandlerTypeFunc = std::function<bool(const std::vector<uint8_t> &, std::vector<uint8_t> &)>;
                const uint16_t cSumationOverVectorMethodId = 1000;
                const uint16_t cMultiplicationOverVectorMethodID = 2000;
                const uint16_t cGetSumMethodID = 3000;

                const uint16_t  cRequestUpdateSessionMethodID  = 4001;
      
                const uint16_t  cPrepareUpdateMethodID  = 4002;
                const uint16_t  cVerifyUpdateMethodID  = 4003;
                const uint16_t  cPrepareRollbackMethodID  = 4004;

                const uint16_t cStopUpdateSessionMethodID = 4005;
                const uint16_t cResetMachineMethodID = 4006; 

                /******************************* constructors  ******************************/
                RPCSServiceProvider::RPCSServiceProvider(
                    uint16_t serviceId,
                    uint16_t instanceId,
                    uint8_t majorVersion,
                    uint8_t minorVersion,
                    uint16_t eventgroupId, 
                    AsyncBsdSocketLib::Poller *poller,
                    std::string nicIpAddress,
                    std::string multicastGroup,
                    uint16_t port,
                    uint16_t endpointRpcsPort,
                    uint8_t protocolVersion,
                    uint8_t interfaceVersion): mServiceId{serviceId},
                                            mInstanceId{instanceId},
                                            mMajorVersion{majorVersion},
                                            mMinorVersion{minorVersion},
                                            mEventgroupId{eventgroupId},
                                            mProtocolVersion{protocolVersion},
                                            mInterfaceVersion{interfaceVersion},
                                            mPoller{poller},
                                            cNicIpAddress{nicIpAddress},
                                            cMulticastGroup{multicastGroup},
                                            cPort{port},
                                            mEndpointRpcsPort{endpointRpcsPort},
                                            mUdpSocket(cAnyIpAddress, port, nicIpAddress, multicastGroup)
                {
                    bool _successful{mUdpSocket.TrySetup()};
                    if (!_successful)
                    {
                        throw std::runtime_error("UDP socket setup failed.");
                    }

                    auto _receiver{std::bind(&RPCSServiceProvider::onReceive, this)};
                    _successful = mPoller->TryAddReceiver(&mUdpSocket, _receiver);
                    if (!_successful)
                    {
                        throw std::runtime_error("Adding UDP socket receiver failed.");
                    }

                    auto _sender{std::bind(&RPCSServiceProvider::onSend, this)};
                    _successful = mPoller->TryAddSender(&mUdpSocket, _sender);
                    if (!_successful)
                    {
                        throw std::runtime_error("Adding UDP socket sender failed.");
                    }
                }


                /******************************* fundemental functions *********************/
                void RPCSServiceProvider::init()
                {
                    /********* initilization for rpcs ****************/
                    rpcServer = new rpc::SocketRpcServer( mPoller,
                                                         "127.0.0.1",
                                                          mEndpointRpcsPort,
                                                          mProtocolVersion,
                                                          mInterfaceVersion);


                    HandlerTypeFunc handler1 = std::bind(&RPCSServiceProvider::RequestUpdateSession, this, std::placeholders::_1, std::placeholders::_2);
                    rpcServer->SetHandler(mServiceId, cRequestUpdateSessionMethodID, handler1);

                    HandlerTypeFunc handler2 = std::bind(&RPCSServiceProvider::PrepareUpdate, this, std::placeholders::_1, std::placeholders::_2);
                    rpcServer->SetHandler(mServiceId, cPrepareUpdateMethodID, handler2);

                    HandlerTypeFunc handler3 = std::bind(&RPCSServiceProvider::VerifyUpdate, this, std::placeholders::_1, std::placeholders::_2);
                    rpcServer->SetHandler(mServiceId, cVerifyUpdateMethodID, handler3);
                    
                    HandlerTypeFunc handler4 = std::bind(&RPCSServiceProvider::PrepareRollback, this, std::placeholders::_1, std::placeholders::_2);
                    rpcServer->SetHandler(mServiceId, cPrepareRollbackMethodID, handler4);

                    HandlerTypeFunc handler5 = std::bind(&RPCSServiceProvider::StopUpdateSession, this, std::placeholders::_1, std::placeholders::_2);
                    rpcServer->SetHandler(mServiceId, cStopUpdateSessionMethodID, handler5);

                    HandlerTypeFunc handler6 = std::bind(&RPCSServiceProvider::ResetMachine, this, std::placeholders::_1, std::placeholders::_2);
                    rpcServer->SetHandler(mServiceId, cResetMachineMethodID, handler6);
                }


                void RPCSServiceProvider::offerService()
                {                    
                    // make SOMEIP/SD message
                    SomeIpSdMessage mOfferServiceMessage;

                    // prepare offering entry
                    auto _offerServiceEntry
                    {
                        entry::ServiceEntry::CreateOfferServiceEntry( mServiceId,
                                                                      mInstanceId,
                                                                      mMajorVersion,
                                                                      mMinorVersion
                                                                    )
                    };

                    helper::Ipv4Address ipAddress("127.0.0.1");

                    // prepare endpoint option
                    auto _offerEndpointOption
                    {
                        option::Ipv4EndpointOption::CreateUnitcastEndpoint( false,
                                                                            ipAddress,
                                                                            option::Layer4ProtocolType::Tcp,
                                                                            mEndpointRpcsPort
                                                                          )
                    };

                    // add created option to created entry 
                    _offerServiceEntry->AddFirstOption(std::move(_offerEndpointOption));
                    
                    // add created entry to created message
                    mOfferServiceMessage.AddEntry(std::move(_offerServiceEntry));

                    // send the message
                    Send(mOfferServiceMessage);
                }


                void RPCSServiceProvider::stopService()
                {
                     // make SOMEIP/SD message
                    SomeIpSdMessage mStofServiceMessage;

                    // prepare offering entry
                    auto _offerServiceEntry
                    {
                        entry::ServiceEntry::CreateStopOfferEntry( mServiceId,
                                                                   mInstanceId,
                                                                   mMajorVersion,
                                                                   mMinorVersion
                                                                 )
                    };

                    helper::Ipv4Address ipAddress("127.0.0.1");

                    // prepare endpoint option
                    auto _offerEndpointOption
                    {
                        option::Ipv4EndpointOption::CreateUnitcastEndpoint( false,
                                                                            ipAddress,
                                                                            option::Layer4ProtocolType::Tcp,
                                                                            mEndpointRpcsPort
                                                                          )
                    };

                    // add created option to created entry 
                    _offerServiceEntry->AddFirstOption(std::move(_offerEndpointOption));
                    
                    // add created entry to created message
                    mStofServiceMessage.AddEntry(std::move(_offerServiceEntry));

                    // send the message
                    Send(mStofServiceMessage);
                }

                
                /**************************** poller functions  **********************************/
                void RPCSServiceProvider::onReceive()
                {
                    // define array to receive serialized SOMEIP/SD message
                    std::array<uint8_t, cBufferSize> _buffer;

                    std::string _ipAddress;
                    uint16_t _port;

                    // receive serialized SOMEIP/SD message in form of array not vector
                    ssize_t _receivedSize{mUdpSocket.Receive(_buffer, _ipAddress, _port)};
                    if (_receivedSize > 0 && _port == cPort && _ipAddress == cNicIpAddress)
                    {
                        const std::vector<uint8_t> cRequestPayload(
                            std::make_move_iterator(_buffer.begin()),
                            std::make_move_iterator(_buffer.begin() + _receivedSize));

                        // Create the received message from the received payload
                        sd::SomeIpSdMessage _receivedMessage{sd::SomeIpSdMessage::Deserialize(cRequestPayload)};
                        
                        // call function that contain what to do with received message
                        //InvokeEventHandler(std::move(_receivedMessage));
                    }
                }

                void RPCSServiceProvider::onSend()
                {
                    while (!mSendingQueue.Empty())
                    {
                        //std::cout << "------------ onSend ------------\n";
                        std::vector<uint8_t> _payload;
                        bool _dequeued{mSendingQueue.TryDequeue(_payload)};
                        if (_dequeued)
                        {       
                            // for printing
                            std::cout << "\n------------------------------------------------\n";
                            std::cout << ".....sent message..... \n";
                            SomeIpSdMessage toSend  = SomeIpSdMessage::Deserialize(_payload);
                            toSend.print();
                            std::cout << "--------------------------------------------------\n";
                            
                            std::array<uint8_t, cBufferSize> _buffer;
                            std::copy_n(
                                std::make_move_iterator(_payload.begin()),
                                _payload.size(),
                                _buffer.begin());

                            mUdpSocket.Send(_buffer, cMulticastGroup, cPort);
                        }
                    }
                }


                void RPCSServiceProvider::Send(const SomeIpSdMessage &message)
                {
                    std::vector<uint8_t> _payload{message.Payload()};
                    mSendingQueue.TryEnqueue(std::move(_payload));
                }


                AsyncBsdSocketLib::Poller* RPCSServiceProvider::getPoller()
                {
                    return mPoller;
                }


                /******************* functions *****************************/
                bool RPCSServiceProvider::RequestUpdateSession(const std::vector<uint8_t> &input, std::vector<uint8_t> &output) 
                {
                    std::cout << "\nRequestUpdateSession is called\n";

                    bool funcResult = RequestUpdateSessionImp();

                    output.push_back(funcResult); // Put the sum in the output vector

                    return true;
                }

                bool RPCSServiceProvider::PrepareUpdate(const std::vector<uint8_t> &input, std::vector<uint8_t> &output) 
                {
                    std::cout << "\nPrepareUpdate is called\n";

                    bool funcResult = PrepareUpdateImp(input);

                    output.push_back(funcResult); // Put the sum in the output vector

                    return true;
                }

                bool RPCSServiceProvider::VerifyUpdate(const std::vector<uint8_t> &input, std::vector<uint8_t> &output) 
                {
                    std::cout << "\nVerifyUpdate is called\n";

                    bool funcResult = VerifyUpdateImp(input);

                    output.push_back(funcResult); // Put the sum in the output vector

                    return true;
                }
                
                bool RPCSServiceProvider::PrepareRollback(const std::vector<uint8_t> &input, std::vector<uint8_t> &output) 
                {
                    std::cout << "\nPrepareRollback is called\n";

                    bool funcResult = PrepareRollbackImp(input);

                    output.push_back(funcResult); // Put the sum in the output vector

                    return true;
                }
                
                bool RPCSServiceProvider::StopUpdateSession(const std::vector<uint8_t> &input, std::vector<uint8_t> &output)
                {
                    StopUpdateSessionImp();
                    return false;
                }

                bool RPCSServiceProvider::ResetMachine(const std::vector<uint8_t> &input, std::vector<uint8_t> &output)
                {
                    ResetMachineImp();
                    return false;
                }

                /**************************** deconstructor  ************************/
                RPCSServiceProvider::~RPCSServiceProvider()
                {
                    mPoller->TryRemoveSender(&mUdpSocket);
                    mPoller->TryRemoveReceiver(&mUdpSocket);
                    delete mPoller;
                }
            }
        }
    }
}