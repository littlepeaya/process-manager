#include "Libraries/JsonConfiguration/JsonConfiguration.hpp"

#include <fcntl.h>

#include "Libraries/Log/LogPlus.hpp"
#include "Generic.hpp"


JsonConfiguration *JsonConfiguration::json_configuration_ = nullptr;
pthread_mutex_t JsonConfiguration::json_configured_mutex_ = PTHREAD_MUTEX_INITIALIZER;

JsonConfiguration::JsonConfiguration() : 
                                        modifying_mutex_(PTHREAD_MUTEX_INITIALIZER),
                                        modified_time_{0},
                                        is_open_(false) {
    writer_builder_["commentStyle"] = "None";
    writer_builder_["identation"] = "";  
}

JsonConfiguration *
JsonConfiguration::GetInstance() {
    pthread_mutex_lock(&json_configured_mutex_);
    if (json_configuration_ == nullptr)
        json_configuration_ = new JsonConfiguration;

    pthread_mutex_unlock(&json_configured_mutex_);

    return json_configuration_;
}

int 
JsonConfiguration::Open(std::string pathname) {
    if (is_open_ == true) {
        LOG_ERRO("The class has been been opened\n");
        return -1;
        
    }

    configuration_pathname_ = pathname;
    if (Update() < 0) {
        LOG_ERRO("Error updating the configuration file\n");
        goto out;
    }
    
    is_open_ = true;
    return 0;
    
out: 
    configuration_pathname_.clear();
    return -1;
}

void 
JsonConfiguration::Close() {
    json_configuration_data_.clear();
    is_open_ = false;
}

Json::Value
JsonConfiguration::Read() {
    pthread_mutex_lock(&modifying_mutex_);
    if (Update() < 0) {
        pthread_mutex_unlock(&modifying_mutex_);
        return Json::nullValue;
    }
    pthread_mutex_unlock(&modifying_mutex_);

    return json_configuration_data_;
}

int
JsonConfiguration::Write(const Json::Value& data) {
    int retval;
    
    std::string copied_pathname = configuration_pathname_ + ".copied";
    int copyfd = open(copied_pathname.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (copyfd < 0) {
        LOG_ERRO("Error creating the copied configuration file\n");
        return -1;
    }

    std::string writing_data = Json::writeString(writer_builder_, data);
    LOG_INFO("%s\n", writing_data.c_str());

    pthread_mutex_lock(&modifying_mutex_);
    retval = write(copyfd, writing_data.c_str(), writing_data.length());
    if (retval !=  writing_data.length()) {
        LOG_ERRO("Error writing the '%s' file\n", copied_pathname.c_str());
        retval = -1;
        goto out;
    }

    retval = Copy(copied_pathname.c_str(), configuration_pathname_.c_str());
    close(retval);
    if (retval < 0)
        goto out;
        
    struct stat statbuf;
    stat(configuration_pathname_.c_str(), &statbuf);
    modified_time_ = statbuf.st_mtim;
    json_configuration_data_ = data;
    retval = 0;
    sync();

out:
    close(copyfd);
    pthread_mutex_unlock(&modifying_mutex_);
    return retval;
}

int 
JsonConfiguration::Update() {
    std::string errs;
    Json::CharReaderBuilder reader_builder;
    reader_builder["collectComments"] = false;
    const std::unique_ptr<Json::CharReader> reader(reader_builder.newCharReader());
    std::string configuration_data;
    struct stat statbuf;
    char data[4096];
    int retval;

    stat(configuration_pathname_.c_str(), &statbuf);
    if (modified_time_.tv_sec >= statbuf.st_mtim.tv_sec && 
            modified_time_.tv_nsec >= statbuf.st_mtim.tv_nsec)
        return 0;

    int fd = open(configuration_pathname_.c_str(), O_RDONLY);
    if (fd < 0) {
        LOG_ERRO("Could not open the '%s' file\n", configuration_pathname_.c_str());
        return -1;
    }

    while (true) {
        memset(data, 0, sizeof(data));

        retval = read(fd, data, sizeof(data));
        if (retval < 0) {
            LOG_ERRO("Error reading the '%s' file, err: %s\n", configuration_pathname_.c_str(), strerror(errno));
            retval = -1;
            goto out;
        }

        if (retval == 0)
            break;

        configuration_data.append(data);
    }

    json_configuration_data_.clear();
    if (reader->parse(configuration_data.c_str(), configuration_data.c_str() + configuration_data.length(), &json_configuration_data_, &errs) == false) {
        LOG_ERRO("Error parse the '%s' configuration file, err: %s\n", configuration_pathname_.c_str(), errs.c_str());
        retval = -1;
        goto out;
    }

    modified_time_ = statbuf.st_mtim;
    retval = 0;

out:
    close(fd);
    return retval;
}

std::string 
JsonConfiguration::GetPathname() {
    return configuration_pathname_;
}