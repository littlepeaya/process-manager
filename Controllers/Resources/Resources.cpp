#include "Resources.hpp"

#define MAXIMUM_COUNT_CPU_TO_REBOOT 100 
#define MAXIMUM_COUNT_RAM_TO_REBOOT 30 

int Resources::check_RAM_ = 0;
int Resources::check_CPU_ = 0; 
KeepAlive Resources::keep_alive_  ; 
LogHistory Resources::log_transfer_; 

Resources::Resources() : percent_cpu_avg_(0),  
                         free_ram_(0), 
                         count_(0),
                         count_cpu_(0), 
                         ready_restart_(false), 
                         cpu_limitted_(0), 
                         ram_limmited_(0), 
                         mem_info_(), 
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
    LOG_INFO("%d", __LINE__); 
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
    auto config = JsonConfiguration::GetInstance()->Read();
    Service ser; 
    for(int i = 0; i < config["services"].size(); ++i) {
        ser.logpath = config["services"][i]["pathlog"].asString(); 
        ser.name = config["services"][i]["name"].asString(); 
        ser.priority = config["services"][i]["priority"].asInt(); 

        service_.push_back(ser); 
    }
    for (int i = 0; i < service_.size(); ++i) {
        for(int j = 1; j < service_.size(); ++j) {
            if(service_[i].priority < service_[j].priority) {
                ser.priority = service_[i].priority; 
                ser.name = service_[i].name; 
                ser.logpath = service_[i].logpath; 

                service_[i].logpath = service_[j].logpath; 
                service_[i].name = service_[j].name; 
                service_[i].priority = service_[j].priority; 

                service_[j].logpath = ser.logpath; 
                service_[j].name = ser.name; 
                service_[j].priority = ser.priority; 
            }
        }
    }
    if(!is_stable_)
        is_stable_ = true; 
    LOG_DBUG("%d", __LINE__); 
    ram_limmited_ = config["resources"]["ram"].asInt(); 
    cpu_limitted_ = config["resources"]["cpu"].asInt(); 
    LOG_INFO("RAM LIMITTED: %d MB", ram_limmited_); 
    LOG_INFO("CPU LIMITTED: %d percent", ram_limmited_); 
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
        percent_cpu_init[time_count] =(static_cast<float>(stats_all - stats[CP_IDLE]) /static_cast<float>( stats_all )) * 100.00;
        usleep(1000); 
        time_count++; 
    }
    for( int i = 0; i < TIME_COUNT; ++i) {
        percent_cpu += percent_cpu_init[i]; 
    }
    data->percent_cpu_avg_ = static_cast<float>(percent_cpu/TIME_COUNT);   
    
    if(data->percent_cpu_avg_ >= LIMIT_CPU_IN_USE) { 
        data->count_cpu_++; 
        LOG_INFO("CPU is %0.2f", data->percent_cpu_avg_ ); 
        LOG_WARN("CPU is over high at %s", ctime(&time_)); 
        LOG_WARN("Trying restart services dependence priority"); 
        for(int i = 0; i < data->service_.size(); ++i) {
            LOG_INFO("restart service %d %s", data->service_[i].priority, data->service_[i].name.c_str()); 
            keep_alive_.RestartService(data->service_[i].name);
        }
        
    } 
    else 
        data->count_cpu_ = 0; 
    if(data->count_cpu_ >= MAXIMUM_COUNT_CPU_TO_REBOOT ) {
        LOG_ERRO("CPU is very high during 1 hour. Upload Log and reboot device"); 
        log_transfer_.LogTransfer(data); 
        data->count_cpu_ = 0; 
        // reboot(LINUX_REBOOT_CMD_RESTART); 
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
    data->free_ram_ = 0; 
    unsigned int actually_in_used_mem = 0; 
    std::ifstream memory_info("/proc/meminfo");

    while (std::getline ( memory_info, line)) {
        substr_start = 0;
        substr_len = line.find_first_of(':'); 
        substr = line.substr(substr_start, substr_len); 
        substr_start = line.find_first_not_of(" ", substr_len + 1); 
        substr_len = line.find_first_of('k') - substr_start; 

        if( std::strcmp( substr.c_str(), "MemTotal") == 0) {
            data->mem_info_.mem_total = std::stoi(line.substr(substr_start, substr_len));
        }
        if (std::strcmp(substr.c_str(), "MemFree") == 0) {
            data->mem_info_.mem_free = std::stoi(line.substr(substr_start, substr_len));
        }
        if (std::strcmp(substr.c_str(), "Buffers") == 0) {
            data->mem_info_.buffers = std::stoi(line.substr(substr_start, substr_len));
        }
        if (std::strcmp(substr.c_str(), "Cached") == 0) {
            data->mem_info_.cached = std::stoi(line.substr(substr_start, substr_len));
        }
        if (std::strcmp(substr.c_str(), "MemAvailable") == 0) {
            data->mem_info_.mem_available = std::stoi(line.substr(substr_start, substr_len));
        }
    }
    data->free_ram_ = (static_cast<int>(data->mem_info_.mem_available + data->mem_info_.cached) / 1024); // convert to MB
    LOG_INFO("ram free is %d", data->free_ram_);
    if(data->free_ram_ >= LIMIT_RAM_FREE) {
    std::string command;
    command = "sync; ";
    command += "echo 3 > /proc/sys/vm/drop_caches ; ";
    data->LogTransfer(data);
    ++data->count_;
    } 
    else {
        data->count_ = 0; 
    }

    if(data->count_ >= MAXIMUM_COUNT_RAM_TO_REBOOT) {
        LOG_WARN("RAM is not free. Trying reboot ..."); 
        data->count_ = 0; 
        LOG_INFO("==============="); 
        // reboot(LINUX_REBOOT_CMD_RESTART);  
    }  
}


int
Resources::LoadAverages(void *user_data) {
    auto data = (Resources *) user_data; 
    double load_avg[3];
    getloadavg(load_avg, 3); 
    if(load_avg[1] > CORE) {
        LOG_WARN("The system has a problem. Trying reboot ..."); 
        reboot(LINUX_REBOOT_CMD_RESTART);   
    }
}



