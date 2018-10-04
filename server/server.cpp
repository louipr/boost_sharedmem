#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <cstdio> 

#include "../lib/boostShm.h"
using namespace std; 
boostShm *gbShm; 
void my_handler(int sid){
           printf("Caught signal %d\n",sid);
           gbShm->~boostShm(); 
           exit(1); 

}


int main()
{
    signal (SIGINT,my_handler);
    cout << "Starting Server" << endl; 
    boostShm bShm(4,"sharedmem",mem_server);
    gbShm = &bShm; //destructor work around; 
    bShm.startServer();
    bShm.mem_write();
    return 0; 
}

