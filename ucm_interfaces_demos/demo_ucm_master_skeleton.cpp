// headers
#include <iostream>
#include "../ucm_interfaces/ucmMaster_ucm_skeleton.h"
#include <thread>


// namespaces
using namespace ara::com::someip::sd;
using namespace ara::com::helper;
using namespace AsyncBsdSocketLib;
using namespace ara::com;
using namespace ara::com::skelton;


// global variables
bool executing = true;
Poller* poller;
const int cTimeoutMs = 100;


int main()
{
   // Create Object from Skeleton 
   InstanceIdentifier id("14");
   UCMMASTER_UCM_Skelton mySkeleton(id);


   // Create thread using a lambda expression
   poller = new Poller();
   poller = mySkeleton.getPoller();
   std::thread t1([](){
      while(executing)
      {
        poller->TryPoll(cTimeoutMs);
      }
   });


   // initilize the service before offer it
   mySkeleton.init();


   // offer the service
   std::cout << "before offering the service\n";
   mySkeleton.offerService();
   std::cout << "after offering the service\n";


   // Join the thread with the main thread
   t1.join();
   return 0;
}