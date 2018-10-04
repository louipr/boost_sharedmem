#include <iostream>
#include <exception> 
#include <thread>
#include <chrono>
#include "../lib/boostShm.h"
using namespace std; 


int main()
{
    cout << "Starting Client" << endl; 
    boostShm bShm(4,"sharedmem",mem_client);
    int fail_count = 0; 
    const unsigned int max_fails = 30; 
    while(1)
    {
        try
        {

            cout << "[Client] Starting client" << endl;
            this_thread::sleep_for(std::chrono::milliseconds(250));
            bShm.startClient();
            bShm.mem_read();
            if(bShm.is_link_ok() == false) fail_count++;
            if(fail_count > max_fails)
            {
                cout << "[Client] Max number of reconnects exceeded" << endl; 
                cout << "[Client] Exiting program" << endl; 
                break; 
            }
        }
        catch(...)
        {
            cout << "main() Exception handler" << endl; 
            fail_count++;
            if(fail_count > max_fails)
            {
                cout << "[Client] Max number of reconnects exceeded" << endl; 
                cout << "[Client] Exiting program" << endl; 
                throw 777; 
            }
        }
    }
    return 0; 
}


