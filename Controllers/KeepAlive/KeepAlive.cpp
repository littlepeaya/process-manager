#include "Controllers/KeepAlive/KeepAlive.hpp" 


KeepAlive::KeepAlive() :
                        active_(0) {

}


KeepAlive::~KeepAlive(){

}

int 
KeepAlive::Start() { 
    if(!active_) {
    auto configuration = JsonConfiguration::GetInstance()->Read();
    std::string service; 
    for(int i = 0; i < configuration["services"].size(); ++i) {
        service = configuration["services"][i].asString();
        StartService(service);
        LOG_INFO("Service %s: Start", service.c_str());   
    }
    return 0; 
    }
}

void 
KeepAlive::Stop() {

}

void
KeepAlive::StartService(const std::string &name) {
    std::string command; 
    command = "/etc/init.d/" + std::string(name) + " " + "start";
    std::string result = ExecuteCommand(command.c_str()); 

    LOG_DBUG("%s",result.c_str());
}

void KeepAlive::HandleKeepAlive(std::string name) {
    std::string command;
    command = "pidof " + std::string(name);
    int ret = std::stoi(ExecuteCommand(command.c_str())); 
    if (ret) {
    LOG_INFO("Service %s is active", name.c_str());
    }
    else { //ret = 0 meaning no process of service 
    LOG_DBUG("Service %s is die. Trying wake up", name.c_str());
    }
    sleep(10);
}

void 
KeepAlive::StopService(const std::string &name) {
     std::string command; 
    command = "/etc/init.d/" + std::string(name) + " " + "stop";
    ExecuteCommand(command.c_str());
}



std::string
KeepAlive::ExecuteCommand(const char *cmd) {
    try {
        std::array<char, 128> buffer{};
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            THROW_EXCEPTION();  
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }catch (std::exception& ex) {
        LOG_ERRO(ex.what());
        return String();
    }
}

    