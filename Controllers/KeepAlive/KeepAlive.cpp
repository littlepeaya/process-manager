#include "KeepAlive.hpp" 



KeepAlive::KeepAlive() :
                        active_(0),
{}

KeepAlive::~KeepAlive()
{}

int 
KeepAlive::Start() { 
    if(!active_) {
    auto configuration = JsonConfiguration::GetInstance()->Read();
    std::string service; 
    for(int i = 0; i < configuration["service"].size(); ++i) {
        service = configuration["service"][i].asString();
        StartService(service);
            
    }

}
}

void 
KeepAlive::Stop() {

}

int 
KeepAlive::StartService(const std::string &name) {
    std::string command; 
    command = "/etc/init.d/" + std::string(name) + " " + "start";
    std::string result = ExecuteCommand(command.c_str());

    if (result == "FAIL") 
        LOG_ERRO("Could not start service %s", name.c_str()); 
    else 
        LOG_INFO("Service %s is activity", name.c_str()); 

    return getpid(); 
}

void 
KeepAlive::HandleStatus() {
    
    


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

    