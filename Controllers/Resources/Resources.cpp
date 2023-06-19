#include "Resources.hpp"

#define MAXIMUM_COUNT_CPU_TO_REBOOT 5
#define MAXIMUM_COUNT_RAM_TO_REBOOT 20
#define VALUE_OF_SAMPLE 5 

int Resources::check_RAM_ = 0;
int Resources::check_CPU_ = 0; 
KeepAlive Resources::keep_alive_; 
LogHistory Resources::log_transfer_; 

Resources::Resources() : percent_cpu_avg_(0),  
                         count_(0),
                         service_(),
                         free_ram_(0), 
                         count_ram_(0),
                         count_cpu_(0), 
                         cpu_limitted_(0), 
                         ram_limmited_(0), 
                         file_ram_(),
                         file_cpu_(), 
                         core_(0), 
                         data_ram_(),
                         data_cpu_(),
                         mem_info_() { 
    timer_check_RAM_.RegisterTimerHandler(HandleStatusRAMIsOver,this); 
    timer_check_Load_Averages_.RegisterTimerHandler(LoadAverages, this); 
    timer_check_CPU_.RegisterTimerHandler(HandleStatusCPUusage, this); 
}

Resources::~Resources() {
}

int 
Resources::Start() {
    auto config = JsonConfiguration::GetInstance()->Read();
    Service ser; 
    for (auto &name : config["services"].getMemberNames()) {
        ser.logpath = config["services"][name]["pathlog"].asString(); 
        ser.priority = config["services"][name]["priority"].asInt(); 
        ser.execute = config["services"][name]["execute"].asString(); 
        ser.kill = config["services"][name]["kill"].asString(); 

        service_.insert(std::pair<std::string, Service> (name,ser)); 
    }
    core_ = config["core"].asInt(); 
    ram_limmited_ = config["resources"]["ram"].asInt(); 
    cpu_limitted_ = config["resources"]["cpu"].asInt(); 
    file_ram_ = config["resourses"]["dataram"].asString();
    file_cpu_ = config["resourses"]["datacpu"].asString();
    try {
        std::string date = GetLocalTime(); 
        data_cpu_ = open("/var/log/datacpu.log", O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0644);
        data_ram_ = open("/var/log/dataram.log", O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0644);
        std::string buff = "write data from boot machine at " + date + "\n";
        write(data_cpu_, buff.c_str(), buff.length());
        write(data_ram_, buff.c_str(), buff.length());
    }catch (std::exception& ex) {
        LOG_ERRO(ex.what());
    }
    LOG_CRIT("==========Resources module===============");
    LOG_INFO("RAM LIMITTED: %d MB", ram_limmited_); 
    LOG_INFO("CPU LIMITTED: %d percent", cpu_limitted_); 

    if(timer_check_RAM_.Start(100,TIME_CHECK_RAM) < 0 ) {
        LOG_ERRO("Could not start timer to check RAM"); 
        return -1; 
    } 
    if(timer_check_CPU_.Start(100, TIME_CHECK_CPU) < 0) {
        LOG_ERRO("Could not start timer to check CPU"); 
    }
    if(timer_check_Load_Averages_.Start(100, TIME_CHECK_LOADAVG) < 0 ) {
        LOG_ERRO("Could not start timer to check CPU"); 
        return -1; 
    }
    return 1; 
}

void 
Resources::Stop() {
    timer_check_Load_Averages_.Stop(); 
    timer_check_Load_Averages_.CancelTimerHandler(LoadAverages); 

    timer_check_RAM_.Stop(); 
    timer_check_RAM_.CancelTimerHandler(HandleStatusRAMIsOver); 

    timer_check_CPU_.Stop(); 
    timer_check_CPU_.CancelTimerHandler(HandleStatusCPUusage); 
}

int 
Resources::HandleStatusCPUusage(void *user_data) {
    auto data = (Resources *) user_data;
    time_t time_; 
    int retval;
    int nums_of_services; 
    time_ = time(NULL); 
    std::string line;
    size_t substr_start = 0;
    size_t substr_len;
    float percent_cpu_sub[TIME_COUNT]; 
    float percent_cpu = 0.00; 
    int max = 6; 
    char buff[max]; 
    int f_sample = 0;

    unsigned long long stats[CP_STATES];
    unsigned long long stats_all; 
    int time_count = 0;
    std::ifstream stat_file("/proc/stat");
    if(stat_file.fail()) {
        LOG_DBUG("Open file state failure"); 
        goto out; 
    }

    while (time_count != TIME_COUNT) {
        std::ifstream stat_file("/proc/stat");
        percent_cpu_sub[time_count] = 0; 
        stats_all = 0; 
        getline(stat_file, line);
        substr_len = line.find_first_of(" ", 3);

        for (unsigned i = 0; i < 4; ++i) {
            substr_start = line.find_first_not_of(" ", substr_len);
            substr_len = line.find_first_of(" ", substr_start);
            stats[i] = std::stoll(line.substr(substr_start, substr_len));
            stats_all += stats[i];
        }
        percent_cpu_sub[time_count] =(static_cast<float>(stats_all - stats[CP_IDLE]) /static_cast<float>( stats_all )) * 100.00;
        if( percent_cpu_sub[time_count] >= data->cpu_limitted_ ){
            f_sample++; 
        }
        gcvt(percent_cpu_sub[time_count], sizeof(percent_cpu_sub[time_count]), buff); 
        write(data->data_cpu_, buff, sizeof(buff));
        write(data->data_cpu_, " ", 1);
        sleep(1); //1s
        time_count++; 
    } 
    for( int i = 0; i < TIME_COUNT; ++i) {
        percent_cpu += percent_cpu_sub[i]; 
    }
    data->percent_cpu_avg_ = static_cast<float>(percent_cpu/TIME_COUNT);   
    LOG_INFO("CPUavg in used is %0.2f during 10s", data->percent_cpu_avg_); 
    
    if(data->percent_cpu_avg_ >= data->cpu_limitted_ && f_sample >= VALUE_OF_SAMPLE) { 
        LOG_WARN("CPU is over high at %s", ctime(&time_)); 
        LOG_WARN("Trying restart all services dependence priority"); 
        for (auto &itr : data->service_) {
            LOG_INFO("Restart service %d %s", itr.second.priority, itr.first.c_str()); 
            keep_alive_.RestartService(itr.first); 
        }   
        ++data->count_cpu_;   
    } 
    else 
        data->count_cpu_ = 0; 
    if(data->count_cpu_ >= MAXIMUM_COUNT_CPU_TO_REBOOT ) {
        LOG_ERRO("CPU is very high during 1 hour. Upload Log and reboot device"); 
        log_transfer_.LogTransfer(data);   
        reboot(LINUX_REBOOT_CMD_RESTART); 
    }
out: 
    stat_file.close(); 
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
    char buff[5]; 
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
    data->free_ram_ = (static_cast<int>(data->mem_info_.mem_available ) / 1024); // convert to MB
    LOG_INFO("RAM free is %d", data->free_ram_);
    try {
        gcvt(data->free_ram_, sizeof(data->free_ram_), buff); 
        write(data->data_ram_, buff, sizeof(buff));
        write(data->data_ram_, " ", 1); 
    }catch (std::exception& ex) {
        LOG_ERRO(ex.what());
    }
    if(data->free_ram_ <= data->ram_limmited_) {
        std::string command;
        command = "sync; echo 3 > /proc/sys/vm/drop_caches";
        Execute(command); 
        LOG_INFO("push log into server %d", data->count_ram_); 
        data->LogTransfer(data);
        ++data->count_ram_;
    }else {
        data->count_ram_ = 0; 
    }
    if(data->count_ram_ >= MAXIMUM_COUNT_RAM_TO_REBOOT) {
        LOG_WARN("RAM is not free. Trying reboot ..."); 
        reboot(LINUX_REBOOT_CMD_RESTART);  
    }  
}

int
Resources::LoadAverages(void *user_data) {
    auto data = (Resources *) user_data; 
    double load_avg[3];
    getloadavg(load_avg, 3); 
    if(load_avg[1] > data->core_) {
        LOG_WARN("The system has a problem. Trying reboot ..."); 
        reboot(LINUX_REBOOT_CMD_RESTART);   
    }
}
