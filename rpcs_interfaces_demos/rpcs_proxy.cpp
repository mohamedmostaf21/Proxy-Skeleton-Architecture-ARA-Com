// headers
#include <iostream>
#include "../rpcs_interfaces/RPCSProxy.h"
#include <thread>
#include <chrono> // for delay


// namespaces
using namespace ara::com::someip::sd;
using namespace ara::com::helper;
using namespace AsyncBsdSocketLib;
using namespace ara::com;
using namespace ara::com::proxy;


// global variables
bool executing = true;
Poller* poller;
const int cTimeoutMs = 100;


int main()
{
    std::cout << "-----------------------------------------------------------\n";
    std::cout << "before finding \n";
    std::cout << "-----------------------------------------------------------\n";

    InstanceIdentifier id("3");
    auto handles = RPCSProxy::findSerivce(id);
    
    std::cout << "-----------------------------------------------------------\n";
    std::cout << "after finding \n";
    std::cout << "-----------------------------------------------------------\n\n\n";


    if(!handles.empty())
    {
        RPCSProxy myProxy(handles[0]);

        // Create thread using a lambda expression
        poller = handles[0].getRequester()->getPoller();
        std::thread t1([](){
            while(executing)
            {
            poller->TryPoll(cTimeoutMs);
            std::this_thread::yield();
            }
        });


        /*************** part 1 *************/
        /*
        std::vector<uint8_t> output3;
        std::future<bool> futureObj3 = myProxy.RequestUpdateSession(output3);

        if(futureObj3.get())
        {
            std::cout << "result of RequestUpdateSession : ";
            for (uint8_t val : output3) {
            std::cout << static_cast<int>(val) ;
            }
            std::cout << "\n";
        }
        */

        /************* part 2 ****************/
        std::vector<uint8_t> input = {4, 2, 13, 4, 45};

        std::vector<uint8_t> output3;

        std::future<bool> futureObj3 = myProxy.PrepareRollback(input,output3);

        if(futureObj3.get())
        {
            std::cout << "result of PrepareRollback : ";
            for (uint8_t val : output3) {
            std::cout << static_cast<int>(val) ;
            }
            std::cout << "\n";
        }

        /*********** part 3 ******************/
        // myProxy.StopUpdateSession();
        // myProxy.ResetMachine();


        std::cout << "test is done\n";
        t1.join();
    }
    else
    {
        std::cout << "handles are empty\n";
    }

    return 0;   
}