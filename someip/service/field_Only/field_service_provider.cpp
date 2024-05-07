#include "field_service_provider.h"


namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace sd
            {
                /******************************* constructors  ******************************/

                FieldServiceProvider::FieldServiceProvider(
                    uint16_t serviceId,
                    uint16_t instanceId,
                    uint8_t majorVersion,
                    uint8_t minorVersion,
                    uint16_t eventgroupId, 
                    AsyncBsdSocketLib::Poller *poller,
                    std::string nicIpAddress,
                    std::string multicastGroup,
                    uint16_t port,
                    uint16_t endpointEventPort,
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
                                            mEndpointEventPort{endpointEventPort},
                                            mUdpSocket(cAnyIpAddress, port, nicIpAddress, multicastGroup)
                {
                    bool _successful{mUdpSocket.TrySetup()};
                    if (!_successful)
                    {
                        throw std::runtime_error("UDP socket setup failed.");
                    }

                    auto _receiver{std::bind(&FieldServiceProvider::onReceive, this)};
                    _successful = mPoller->TryAddReceiver(&mUdpSocket, _receiver);
                    if (!_successful)
                    {
                        throw std::runtime_error("Adding UDP socket receiver failed.");
                    }

                    auto _sender{std::bind(&FieldServiceProvider::onSend, this)};
                    _successful = mPoller->TryAddSender(&mUdpSocket, _sender);
                    if (!_successful)
                    {
                        throw std::runtime_error("Adding UDP socket sender failed.");
                    }
                }



                /******************************* fundemental functions *********************/
    
                void FieldServiceProvider::init(std::vector<uint8_t> currentValue, Handler h)
                {
                    /************** initialization for events ***************/            
                    
                    fieldServer = new SockeKFieldServer(mServiceId,
                                            mInstanceId,
                                            mMajorVersion,
                                            mEventgroupId,
                                            mPoller,
                                            cNicIpAddress,
                                            cMulticastGroup,
                                            mEndpointEventPort,
                                            mProtocolVersion);
                    
                    bool _result = fieldServer->putCurrentValue(currentValue);
                    if(_result)
                    {
                        fieldServer->SetHandler(h);
                        fieldServer->Start();
                    }
                    else
                    {
                        std::cout << "fail to put current value\n";
                    }
                }


                void FieldServiceProvider::offerService()
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
                                                                            mEndpointEventPort
                                                                          )
                    };

                    // add created option to created entry 
                    _offerServiceEntry->AddFirstOption(std::move(_offerEndpointOption));
                    
                    // add created entry to created message
                    mOfferServiceMessage.AddEntry(std::move(_offerServiceEntry));

                    // send the message
                    Send(mOfferServiceMessage);
                }


                void FieldServiceProvider::stopService()
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
                                                                            mEndpointEventPort
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

                void FieldServiceProvider::onReceive()
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

                void FieldServiceProvider::onSend()
                {
                    while (!mSendingQueue.Empty())
                    {
                        //std::cout << "------------ onSend ------------\n";
                        std::vector<uint8_t> _payload;
                        bool _dequeued{mSendingQueue.TryDequeue(_payload)};
                        if (_dequeued)
                        {       
                            // for printing
                            std::cout << "\n-----------------------------------------\n";
                            std::cout << ".....sent message..... \n";
                            SomeIpSdMessage toSend  = SomeIpSdMessage::Deserialize(_payload);
                            toSend.print();
                            std::cout << "-----------------------------------------\n";
                            
                            std::array<uint8_t, cBufferSize> _buffer;
                            std::copy_n(
                                std::make_move_iterator(_payload.begin()),
                                _payload.size(),
                                _buffer.begin());

                            mUdpSocket.Send(_buffer, cMulticastGroup, cPort);
                        }
                    }
                }


                void FieldServiceProvider::Send(const SomeIpSdMessage &message)
                {
                    std::vector<uint8_t> _payload{message.Payload()};
                    mSendingQueue.TryEnqueue(std::move(_payload));
                }


                AsyncBsdSocketLib::Poller* FieldServiceProvider::getPoller()
                {
                    return mPoller;
                }

                /**************************** deconstructor  ************************/

                FieldServiceProvider::~FieldServiceProvider()
                {
                    mPoller->TryRemoveSender(&mUdpSocket);
                    mPoller->TryRemoveReceiver(&mUdpSocket);
                    delete mPoller;
                }
            }
        }
    }
}