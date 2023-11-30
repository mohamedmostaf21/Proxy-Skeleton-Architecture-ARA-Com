#ifndef SERVICE_REGISTRY_H
#define SERVICE_REGISTRY_H
#include "../asyncbsdsocket/include/udp_client.h"
#include <stdint.h>
#include "../helper/ipv4_address.h"
#include "../option/option.h"
#include "../entry/entry.h"
#include <iostream>
#include <array>
#include "../someip/sd/someip_sd_message.h"
#include "../entry/service_entry.h"
#include "../option/ipv4_endpoint_option.h"
#include "../option/option_deserializer.h"
#include "../someip/rpc/someip_rpc_message.h"

namespace ara {
	namespace com {
		namespace someip {
			namespace sd {
				typedef struct {
					uint16_t s_service_id;
					uint16_t s_instance_id;
					uint8_t s_major_version;
					uint8_t s_minor_version;
					//how to reach to server by its ip address , protocol (TCP or UDP), and port number
					helper::Ipv4Address s_mAddress;
					option::Layer4ProtocolType s_mProtocol;
					uint16_t s_mPort;
				}service_info;

				class ServiceRegistry {
				private:
					static ServiceRegistry* singleton_registry;
					//Service Registry vector that helds information about all offered servicea at any time 
					std::vector<service_info> offered_services;
					//server object to receive
					AsyncBsdSocketLib::UdpClient* server;

					//server IP Address and port
					std::string cIpAddress = "127.0.0.2";
					uint16_t cPort = 9900;

					//function to handle the offer service message tha was received
					void handle_offer(std::unique_ptr<entry::Entry> entry);
					//function to handle the find service message that was received
					void handle_find(std::unique_ptr<entry::Entry> entry, std::string ip, uint16_t port);

				public:
					ServiceRegistry();
					ServiceRegistry(ServiceRegistry& other) = delete;

					void operator=(const ServiceRegistry&) = delete;

					static ServiceRegistry* get_registry_instance();

					//main daemon based method to run the service registry
					void run();
				};

			}
		}
	}
}
#endif // !SERVICE_REGISTRY_H

