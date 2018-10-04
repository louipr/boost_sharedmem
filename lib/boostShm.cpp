/* 
 Boost Shared Memory Wrapper 

 */ 
#include <iostream> 
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstring>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>

#include "boostShm.h"
using namespace boost::interprocess;
using namespace std; 


boostShm::boostShm(int sharedSize,const char *sharedString,mem_mode m_mode)
{
  this->sharedSize = sharedSize;
  this->sharedString = sharedString;
  this->m_mode = m_mode;
  this->link_ok = false; 

  if(this->m_mode == mem_server)
  {
    shared_memory_object::remove(sharedString);
  }
}
boostShm::~boostShm()
{
  if(this->m_mode == mem_server)
  {
    //make sure to set default values
    memset(region->get_address(), 0x0, region->get_size());
    shared_memory_object::remove(sharedString);
  }

  // delete region;
  // delete shm;
}
bool boostShm::is_link_ok()
{
  return link_ok;
}
void boostShm::startServer()
{
  shared_memory_object *shm = new shared_memory_object(create_only, sharedString, read_write);
  //Set size
  shm->truncate(sharedSize + 1); //setup extra space for LINK_CHECK_CMD
  region = new mapped_region(*shm, read_write);
}

void boostShm::startClient()
{

  cout << "[Client] Creating shared memory object" << endl; 
  try
  {
   shared_memory_object *shm = new shared_memory_object(open_only, sharedString, read_only);
   region = new mapped_region(*shm, read_only);

  }
  catch(...)
  {
    cout << "Unknown shared_memory_object exception in startClient()" << endl; 
  }

  cout << "[Client] Created shared memory object" << endl; 
}

void boostShm::mem_write()
{
  memset(region->get_address(), 0x0, region->get_size());
  unsigned int * mem;

  while(1)
  {

    mem = (unsigned int *)region->get_address();
    *mem = LINK_CHECK_CMD;
    mem++; 
    for(int i = 1 ; i < region->get_size(); i++) //skip 1
    {
      cout << "Enter Value" << endl; 
      cin >> *mem;
      cout << *mem << "=>0x" << std::hex << *mem << endl;  
      //*mem = *mem - '0';
      mem++; 
    }
  }
}

void boostShm::mem_read()
{
  unsigned int counter = 0; 
  while(1)
  {
    //Check that memory was initialized to 1
    unsigned int *mem = (unsigned int *)region->get_address();
    if(link_ok)
    {
      cout << "p2" << endl << "region.get_size() = " << std::dec << region->get_size() << endl;
    }

    if(*mem == LINK_CHECK_CMD){ counter = 0;link_ok = true;}//check counter & link
    for(std::size_t i = 0; i < region->get_size(); ++i)
    {
      if(link_ok)
      {
        cout << "addr: " << std::hex << (unsigned int)mem << "=" << std::hex << (unsigned int)*mem << endl;         
      }
      mem++; 
    }
    this_thread::sleep_for(std::chrono::milliseconds(250));
    counter++; 
    if(counter > region->get_size())
    {
      cout << "[Client] LINK issue detected" << endl; 
      link_ok = false; 
      //throw 777;
      break; 
    }

  }  
}