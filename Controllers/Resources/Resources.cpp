#include "Resources.hpp"

int Resources::check_RAM_ = 0;
int Resources::check_CPU_ = 0; 

Resources::Resources() : 
                        is_stable_(true) { 
    timer_check_RAM_.RegisterTimerHandler(HandleStatusRAMIsOver,this); 
    timer_check_CPU_.RegisterTimerHandler(HandleStatusCPUusage,this); 
    timer_check_Load_Avereages_.RegisterTimerHandler(LoadAverages, this); 
}

Resources::~Resources()
{

}

int 
Resources::Start() {
    if(timer_check_CPU_.Start(100, TIME_CHECK) < 0 ) {
        LOG_ERRO("Could not start timer to check CPU"); 
        return -1; 
    } 

    // if(timer_check_RAM_.Start(150,TIME_CHECK) < 0 ) {
    //     LOG_ERRO("Could not start time to check RAM"); 
    //     return -1; 
    // } 
    if(timer_check_Load_Avereages_.Start(150, TIME_CHECK) < 0 ) {
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
        // sleep(0.01); 
    }

    for( int i = 0; i < TIME_COUNT; ++i) {
        percent_cpu += percent_cpu_init[i]; 
    }
    LOG_INFO("Percent CPU avager is: %0.8f percent", (float)percent_cpu/TIME_COUNT); 
}

int  
Resources::HandleStatusRAMIsOver(void *user_data) {
    auto data = (Resources *) user_data; 
    std::string line; 
    size_t substr_start; 
    size_t substr_len; 

    std::ifstream meminfo_file("/proc/meminfo"); 
    getline(meminfo_file, line); 
    meminfo_file.close(); 

    for ( int i = 0; i < 4; ++i ) {
        substr_start = 0; 
        substr_len = line.find("MemTotal:", 10); 
        substr_start = line.find_first_not_of(" ", substr_len); 
    
    }

}

int
Resources::LoadAverages(void *user_data) {
    auto data = (Resources *) user_data; 
    double load_avg[3];
    if (getloadavg(load_avg, 3) != -1) {
        LOG_INFO("CPU load averages in 1  minute: %0.2f", load_avg[0]); 
        LOG_INFO("CPU load averages in 5  minute: %0.2f", load_avg[1]); 
        LOG_INFO("CPU load averages in 15 minute: %0.2f", load_avg[2]); 
    }


    
}



