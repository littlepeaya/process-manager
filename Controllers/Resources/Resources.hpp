#ifndef __MAV3_PROCESS_MANAGER_RESOURCES_HPP__
#define __MAV3_PROCESS_MANAGER_RESOURCES_HPP__

#include<Libraries/Log/LogPlus.hpp>
#include<Libraries/Timer/Timer.hpp>
#include<Controllers/LogHistory/LogHistory.hpp>

#include<unistd.h> 
#include<fstream>
#include<string>

#define LIMIT_RAM_FREE 50 //50MB 
#define LIMIT_CPU_IN_USE 50 // 50 % 
#define TIME_CHECK 1*100 //20s 

#define CP_USER   0
#define CP_NICE   1 
#define CP_SYS    2 
#define CP_IDLE   3
#define CP_STATES 4
#define TIME_COUNT 10 

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

private: 
    static int check_RAM_;
    static int check_CPU_;

    static int HandleStatusRAMIsOver(void *user_data); 
    static int HandleStatusCPUusage(void *user_data); 

    bool is_stable_; 
    Timer timer_check_RAM_;
    Timer timer_check_CPU_; 
    LogHistory log_tranfer_; 
};

#endif //__MAV3_PROCESS_MANAGER_RESOURCES_HPP__