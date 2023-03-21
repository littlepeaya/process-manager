/*
total memory in used formula : = +total + shmem - free - buffer - cached - sreclaimable  
load avagers is help to know process is running and how much processed is in message queue. But it is not im
important as it shows. < following loadavg of linux git > 
*/

#ifndef __MAV3_PROCESS_MANAGER_RESOURCES_HPP__
#define __MAV3_PROCESS_MANAGER_RESOURCES_HPP__

#include<Libraries/Log/LogPlus.hpp>
#include<Libraries/Timer/Timer.hpp>
#include<Controllers/LogHistory/LogHistory.hpp>
#include<Controllers/KeepAlive/KeepAlive.hpp>
#include "Generic.hpp"
#include<unistd.h> 
#include<fstream>
#include<string>
#include<sys/reboot.h> 
#include<linux/reboot.h> 

#define TIME_CHECK 60*1000 //1 minute  

#define CP_USER   0
#define CP_NICE   1 
#define CP_SYS    2 
#define CP_IDLE   3
#define CP_STATES 4

#define TIME_COUNT 10 

typedef struct {
    unsigned long long  mem_total;
    unsigned long long  mem_free;
    unsigned long long  mem_available; 
    unsigned long long  buffers; 
    unsigned long long  cached;
} MemoryStatus; 

class Resources : public LogHistory {
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

    int count_ram_; 
    int count_cpu_; 
    int free_ram_; 
    int core_; 
    float percent_cpu_avg_; 
    Timer timer_check_RAM_; //important 
    Timer timer_check_CPU_; //important 
    Timer timer_check_Load_Averages_; // not important 
    static LogHistory log_transfer_; 
    static KeepAlive keep_alive_; 
    MemoryStatus mem_info_; 
    int cpu_limitted_; 
    int ram_limmited_; 
    std::map<std::string, Service> service_; 
}; 

#endif //__MAV3_PROCESS_MANAGER_RESOURCES_HPP__