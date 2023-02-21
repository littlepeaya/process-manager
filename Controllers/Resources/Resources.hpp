#ifndef __MAV3_PROCESS_MANAGER_RESOURCES_HPP__
#define __MAV3_PROCESS_MANAGER_RESOURCES_HPP__

#include<Libraries/Log/LogPlus.hpp>
#include<Libraries/Timer/Timer.hpp>
#include<Controllers/LogHistory/LogHistory.hpp>
#include<unistd.h> 

#define LIMIT_RAM_FREE 50 //50MB 
#define LIMIT_CPU_IN_USE 70 // 70% 
#define TIME_CHECK 60*60*1000 //1H 

typedef struct {
    float used_mem;
    float total_mem;
} MemnoryStatus; 

enum MEMORY_MODE
{
  MEMORY_MODE_DEFAULT,
  MEMORY_MODE_FREE_MEMORY,
  MEMORY_MODE_USAGE_PERCENTAGE
}; 

class Resources {
public: 
    Resources(); 
    ~Resources();

    int Start();
    void Stop();
    void Run(); 
    void CheckResource(int timepoint, int timeval); 

private: 
    static int check_RAM_;
    static int check_CPU_;

    static int HandleStatusRAMIsOver(void *user_data); 
    static int HandleStatusCPUusage(void *user_data); 

    bool is_stable_; 
    Timer time_;
    LogHistory log_tranfer_; 
};

#endif //__MAV3_PROCESS_MANAGER_RESOURCES_HPP__