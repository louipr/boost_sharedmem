/* 
 Boost Shared Memory Wrapper 

 */ 
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>

#define LINK_CHECK_CMD 0xFFFF

typedef enum {mem_client,mem_server}mem_mode;

using namespace boost::interprocess;

class boostShm
{

    public: 

       boostShm(int sharedSize,const char *sharedString,mem_mode m_mode);
       ~boostShm();
       void startServer(); 
       void startClient(); 
       void mem_write();
       void mem_read();
       bool is_link_ok(); 
    private:

        int sharedSize;
        bool link_ok; 
        const char *sharedString;
        shared_memory_object * shm; 
        mapped_region * region; 
        mem_mode m_mode; 

};
