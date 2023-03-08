#include "Resources.hpp"

int Resources::check_RAM_ = 0;
int Resources::check_CPU_ = 0; 
KeepAlive Resources::keep_alive_ ; 

Resources::Resources() : percent_cpu_avg_(0),  
                         count_(0),
                         ready_restart_(false), 
                         is_stable_(true) { 
    timer_check_RAM_.RegisterTimerHandler(HandleStatusRAMIsOver,this); 
    timer_check_CPU_.RegisterTimerHandler(HandleStatusCPUusage,this); 
    timer_check_Load_Averages_.RegisterTimerHandler(LoadAverages, this); 
}

Resources::~Resources()
{

}

int 
Resources::Start() {
    LOG_CRIT("==========Resources module===============");
    if(timer_check_CPU_.Start(100, TIME_CHECK) < 0 ) {
        LOG_ERRO("Could not start timer to check CPU"); 
        return -1; 
    } 
    if(timer_check_RAM_.Start(100,TIME_CHECK) < 0 ) {
        LOG_ERRO("Could not start time to check RAM"); 
        return -1; 
    } 
    if(timer_check_Load_Averages_.Start(100, TIME_CHECK) < 0 ) {
        LOG_ERRO("Could not start timer to check CPU"); 
        return -1; 
    }
    return 1; 
}

void 
Resources::Stop() {
    is_stable_ = true; 
}

int 
Resources::HandleStatusCPUusage(void *user_data) {
    auto data = (Resources *) user_data;
    time_t time_; 
    time_ = time(NULL); 
    std::string line;
    size_t substr_start = 0;
    size_t substr_len;
    float percent_cpu_init[TIME_COUNT]; 
    float percent_cpu = 0.00; 

    unsigned long long stats[CP_STATES];
    unsigned long long stats_all; 
    int time_count = 0;

    while (time_count != TIME_COUNT) {
        stats_all = 0; 
        std::ifstream stat_file("/proc/stat");
        getline(stat_file, line);
        stat_file.close();

        substr_len = line.find_first_of(" ", 3);

        for (unsigned i = 0; i < 4; i++) {
            substr_start = line.find_first_not_of(" ", substr_len);
            substr_len = line.find_first_of(" ", substr_start);
            stats[i] = std::stoll(line.substr(substr_start, substr_len));
            stats_all += stats[i];

        }
        percent_cpu_init[time_count] =(static_cast<float>(stats_all - stats[CP_IDLE]) /static_cast<float>( stats_all )) * 100.0 ;
        time_count++; 
    }

    for( int i = 0; i < TIME_COUNT; ++i) {
        percent_cpu += percent_cpu_init[i]; 
    }
   
    data->percent_cpu_avg_ = static_cast<float>(percent_cpu/TIME_COUNT);   
     LOG_INFO("CPU is %0.2f", data->percent_cpu_avg_ ); 

    if(data->percent_cpu_avg_ >= LIMIT_CPU_IN_USE) { 
        data->count_++; 
        LOG_WARN("CPU is over high at %s", ctime(&time_)); 
    } 

    if(data->count_ >= OVER_TIMES) {
        LOG_ERRO("CPU is very high during 1 hour. Upload Log and reboot device"); 
        reboot(LINUX_REBOOT_CMD_RESTART); 
    }

}

int  
Resources::HandleStatusRAMIsOver(void *user_data) {
    auto data = (Resources *) user_data; 
    std::string line; 
    std::string substr;
    size_t substr_start;
    size_t substr_len;

    unsigned int total_mem;
    unsigned int actually_in_used_mem = 0; 

    std::ifstream memory_info("/proc/meminfo");

    while (std::getline ( memory_info, line)) {
        substr_start = 0;
        substr_len = line.find_first_of(':'); 
        substr = line.substr(substr_start, substr_len); 
        substr_start = line.find_first_not_of(" ", substr_len + 1); 
        substr_len = line.find_first_of('k') - substr_start; 

        if( std::strcmp( substr.c_str(), "MemTotal") == 0) {
            actually_in_used_mem += std::stoi(line.substr(substr_start, substr_len)); 
        }
        if( std::strcmp( substr.c_str(), "Shmem") == 0) {
            actually_in_used_mem += std::stoi(line.substr(substr_start, substr_len)); 
        }
        if( std::strcmp( substr.c_str(), "MemFree") == 0) {
            actually_in_used_mem -= std::stoi(line.substr(substr_start, substr_len)); 
        }
        if( std::strcmp( substr.c_str(), "Buffers") == 0) {
            actually_in_used_mem -= std::stoi(line.substr(substr_start, substr_len));  
        }
        if( std::strcmp( substr.c_str(), "Cached") == 0) {
            actually_in_used_mem -= std::stoi(line.substr(substr_start, substr_len)); 
        }
        if( std::strcmp( substr.c_str(), "SReclaimable") == 0) {
            actually_in_used_mem -= std::stoi(line.substr(substr_start, substr_len)); 
        }
    }

    actually_in_used_mem = static_cast<int> (actually_in_used_mem/1024);  // convert to MB 
    LOG_INFO("actually memory in used is : %d (megabytes)", actually_in_used_mem); 


}


int
Resources::LoadAverages(void *user_data) {
    auto data = (Resources *) user_data; 
    double load_avg[3];
    getloadavg(load_avg, 3); 
    if(load_avg[1] > CORE) {
        LOG_ERRO("The system has a problem. Trying reboot ..."); 
        reboot(LINUX_REBOOT_CMD_RESTART); 
        
    }
}



