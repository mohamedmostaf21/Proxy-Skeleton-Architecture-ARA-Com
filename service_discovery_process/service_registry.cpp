#include "service_registry.h"
#include <array>
#include <cstdint>
#include <algorithm>
#include <string.h>
namespace ara {
	namespace com {
		namespace someip {

			namespace sd {
				ServiceRegistry::ServiceRegistry() {
					server = new AsyncBsdSocketLib::UdpClient(cIpAddress, cPort);
					bool _succeed = server->TrySetup();
					if (_succeed) {
						std::cout << "Server Port created successfully\n";
						std::cout << "Waiting For Messages\n";

					}
					else {
						std::cout << "Server Port cannot be created\n";

					}
				}
				ServiceRegistry* ServiceRegistry::get_registry_instance() {
					if (singleton_registry == nullptr) {
						singleton_registry = new ServiceRegistry();
					}
					return singleton_registry;
				}

				void ServiceRegistry::run() {
					while (true) {
						std::array<uint8_t, 512> buff;
						std::string recIpAddress;
						uint16_t recPort;
						ssize_t _received = server->Receive(buff, recIpAddress, recPort);
						if (_received) {
							std::cout << "The Server received message from IP Address: " << recIpAddress << " and Port: " << recPort << std::endl;

						}

						std::vector<uint8_t> payload;
						for (int i = 0; i < _received; i++) {
							payload.push_back(buff[i]);
						}
						SomeIpSdMessage message = SomeIpSdMessage::Deserialize(payload);
						
						for (auto& message_entry : message.Entries()) {
							switch (message_entry->Type())
							{
							case entry::EntryType::Finding:
								//handle_find(std::move(std::make_unique<entry::Entry> (message_entry)), recIpAddress, recPort);
								break;
							case entry::EntryType::Offering:
								//handle_offer(std::move(std::make_unique<entry::Entry> (message_entry)));
							default:
								break;

							}
						}
					}
				}

				void ServiceRegistry::handle_offer(std::unique_ptr<entry::Entry> entry) {

					std::cout << "Handling Offer Service Message ..\n";
					if (auto service_entry = dynamic_cast<entry::ServiceEntry*> (entry.get())) {
						service_info info;
						info.s_service_id = service_entry->ServiceId();
						info.s_instance_id = service_entry->InstanceId();
						info.s_major_version = service_entry->MajorVersion();
						info.s_minor_version = service_entry->MinorVersion();
						if (auto ip4_option = dynamic_cast<option::Ipv4EndpointOption*> (service_entry->FirstOptions().at(0).get())) {
							info.s_mAddress = ip4_option->IpAddress();
							info.s_mProtocol = ip4_option->L4Proto();
							info.s_mPort = ip4_option->Port();
						}
						offered_services.push_back(info);

					}

				}
				
				void ServiceRegistry::handle_find(std::unique_ptr<entry::Entry> entry, std::string ip, uint16_t port) {
					std::cout << "Handling Find Service Message...\n";
					if (auto service_entry = dynamic_cast<entry::ServiceEntry*> (entry.get())) {
						uint16_t l_service_id = service_entry->ServiceId();
						uint16_t l_instance_id = service_entry->InstanceId();
						uint16_t l_major_version = service_entry->MajorVersion();
						uint16_t l_minor_version = service_entry->MinorVersion();

						for (auto& info : offered_services) {
							if (l_service_id == info.s_service_id &&
								(l_instance_id == info.s_instance_id || l_instance_id == entry::Entry::cAnyInstanceId) &&
								(l_major_version == info.s_major_version || l_major_version == entry::Entry::cAnyMajorVersion) &&
								(l_minor_version == info.s_minor_version || l_minor_version == entry::Entry::cAnyMinorVersion)) {
								auto ip_opt = option::Ipv4EndpointOption::CreateUnitcastEndpoint(false, info.s_mAddress, info.s_mProtocol, info.s_mPort);
								auto offer_entry = entry::ServiceEntry::CreateOfferServiceEntry(info.s_service_id, info.s_instance_id, info.s_major_version, info.s_minor_version, 0xFFFFFF);

								////push_back(std::move(ip_opt)
								offer_entry->AddFirstOption(std::move(ip_opt));
								sd::SomeIpSdMessage message;
								message.AddEntry(std::move(offer_entry));
								//make serialization for the sd message and put it into someip payload  
								auto payload = message.SomeIpMessage::Payload();//serilization
								//create someip message
								someip::rpc::SomeIpRpcMessage msg2(0xFFFF8100,0x102, 0x0304, 0x01, 0x01, someip::SomeIpReturnCode::eOK, payload);
								
								//make serialization for the someip message
								msg2.someip::SomeIpMessage::Payload();
								std::array<uint8_t, 256> buffer;
								std::vector<uint8_t> retu = msg2.RpcPayload();
								const uint8_t* mes = retu.data();
								//uint32_t mesSize = msg2.Length();
								
								buffer.fill(*mes);
								

								bool _sent = server->Send(buffer, ip, port) > 0;
								if (_sent) {
									std::cout << "The Server Replied With Offer Message";
								}




							}
						}
					}
				}


			}
		}
	}
}