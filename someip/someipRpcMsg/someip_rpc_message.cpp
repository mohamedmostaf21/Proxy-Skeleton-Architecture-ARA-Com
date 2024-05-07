#include <utility>
#include "./someip_rpc_message.h"
#include <iostream>

namespace ara
{
    namespace com
    {
        namespace someip
        {
            namespace rpc
            {
                /******************************* constructors  ******************************/
            
                SomeIpRpcMessage::SomeIpRpcMessage() : SomeIpMessage(0,
                                                                     0,
                                                                     1,
                                                                     1,
                                                                     SomeIpMessageType::Request)
                {}
                

                SomeIpRpcMessage::SomeIpRpcMessage(uint32_t messageId,
                                                   uint16_t clientId,
                                                   uint16_t sessionId,
                                                   uint8_t protocolVersion,
                                                   uint8_t interfaceVersion,
                                                   const std::vector<uint8_t> &rpcPayload) : SomeIpMessage(messageId,
                                                                                                           clientId,
                                                                                                           protocolVersion,
                                                                                                           interfaceVersion,
                                                                                                           SomeIpMessageType::Request,
                                                                                                           sessionId),
                                                                                             mRpcPayload{rpcPayload}
                {}

                SomeIpRpcMessage::SomeIpRpcMessage(uint32_t messageId,
                                                   uint16_t clientId,
                                                   uint16_t sessionId,
                                                   uint8_t protocolVersion,
                                                   uint8_t interfaceVersion,
                                                   std::vector<uint8_t> &&rpcPayload) : SomeIpMessage(messageId,
                                                                                                      clientId,
                                                                                                      protocolVersion,
                                                                                                      interfaceVersion,
                                                                                                      SomeIpMessageType::Request,
                                                                                                      sessionId),
                                                                                        mRpcPayload{std::move(rpcPayload)}
                {}

                SomeIpRpcMessage::SomeIpRpcMessage(uint32_t messageId,
                                                   uint16_t clientId,
                                                   uint16_t sessionId,
                                                   uint8_t protocolVersion,
                                                   uint8_t interfaceVersion,
                                                   SomeIpReturnCode returnCode,
                                                   const std::vector<uint8_t> &rpcPayload) : SomeIpMessage(messageId,
                                                                                                           clientId,
                                                                                                           protocolVersion,
                                                                                                           interfaceVersion,
                                                                                                           returnCode == SomeIpReturnCode::eOK ? SomeIpMessageType::Response : SomeIpMessageType::Error,
                                                                                                           returnCode,
                                                                                                           sessionId),
                                                                                             mRpcPayload{rpcPayload}
                {}

                SomeIpRpcMessage::SomeIpRpcMessage(uint32_t messageId,
                                                   uint16_t clientId,
                                                   uint16_t sessionId,
                                                   uint8_t protocolVersion,
                                                   uint8_t interfaceVersion,
                                                   SomeIpMessageType messageType,
                                                   SomeIpReturnCode returnCode,
                                                   const std::vector<uint8_t> &rpcPayload) : SomeIpMessage(messageId,
                                                                                                           clientId,
                                                                                                           protocolVersion,
                                                                                                           interfaceVersion,
                                                                                                           messageType,
                                                                                                           returnCode,
                                                                                                           sessionId),
                                                                                                           mRpcPayload{rpcPayload}
                {}



                /*************** override virtual functions inherited from parent*************/

                uint32_t SomeIpRpcMessage::Length() const noexcept
                {
                    auto _result{static_cast<uint32_t>(8 + mRpcPayload.size())};
                    return _result;
                }

                std::vector<uint8_t> SomeIpRpcMessage::Payload() const
                {
                    // General SOME/IP header payload insertion
                    std::vector<uint8_t> _result = SomeIpMessage::Payload();
                    _result.insert( _result.end(), mRpcPayload.cbegin(), mRpcPayload.cend());

                    return _result;
                }

                void SomeIpRpcMessage::print() const noexcept
                {
                    std::cout << "------------SOMEIP_RPC message------------\n";
                    SomeIpMessage::print();
                    std::cout << "payload: ";
                    for (uint8_t byte : mRpcPayload) 
                    {
                        std::cout << static_cast<int>(byte) << "\t";
                    }
                    std::cout << "\n";
                }


                /********************************* getters  *********************************/

                const std::vector<uint8_t> &SomeIpRpcMessage::RpcPayload() const
                {
                    return mRpcPayload;
                }



                /**************************** fundemental functions *************************/

                SomeIpRpcMessage SomeIpRpcMessage::Deserialize(
                    const std::vector<uint8_t> &payload)
                {
                    const size_t cHeaderSize{16};

                    size_t _lengthOffset{4};
                    uint32_t _lengthInt{
                        helper::ExtractInteger(payload, _lengthOffset)};
                    auto _length{static_cast<size_t>(_lengthInt)};

                    SomeIpRpcMessage _result;
                    SomeIpMessage::Deserialize(&_result, payload);
                    _result.mRpcPayload =
                        std::vector<uint8_t>(
                            payload.cbegin() + cHeaderSize,
                            payload.cbegin() + cHeaderSize + _length - _lengthOffset);

                    return _result;
                }
            }
        }
    }
}