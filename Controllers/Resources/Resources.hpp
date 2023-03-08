#ifndef __MAV3_PROCESS_MANAGER_RESOURCES_HPP__
#define __MAV3_PROCESS_MANAGER_RESOURCES_HPP__

/*
total memory in used formula : = +total + shmem - free - buffer - cached - sreclaimable  
load avagers is help to know process is running and how much processed is in message queue. But it is not im
important as it shows. < following loadavg of linux git > 
*/

#include<Libraries/Log/LogPlus.hpp>
#include<Libraries/Timer/Timer.hpp>
#include<Controllers/LogHistory/LogHistory.hpp>
#include<Controllers/KeepAlive/KeepAlive.hpp>

#include<unistd.h> 
#include<fstream>
#include<string>
#include<sys/reboot.h> 
#include<linux/reboot.h> 

#define LIMIT_RAM_FREE 50 //50MB 
#define LIMIT_CPU_IN_USE 1.72 // 50 % 
#define TIME_CHECK 1*1000 //1s 
#define CORE 3 

#define CP_USER   0
#define CP_NICE   1 
#define CP_SYS    2 
#define CP_IDLE   3
#define CP_STATES 4
#define TIME_COUNT 10 
#define OVER_TIMES 60*60 //1 hour 

typedef struct {
    unsigned long long  mem_total;
    unsigned long long  mem_free;
    unsigned long long  mem_available; 
    unsigned long long  buffers; 
    unsigned long long  cached;
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
    static int LoadAverages(void *user_data); 

    int count_; 
    bool is_stable_; 
    bool ready_restart_; 
    float percent_cpu_avg_; 
    Timer timer_check_RAM_; //important 
    Timer timer_check_CPU_; //important 
    Timer timer_check_Load_Averages_; // not important 
    LogHistory log_transfer_; 
    static KeepAlive keep_alive_; 
}; 

#endif //__MAV3_PROCESS_MANAGER_RESOURCES_HPP__