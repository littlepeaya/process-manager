#ifndef LIBRARIES_JSON_CONFIGURATION_HPP
#define LIBRARIES_JSON_CONFIGURATION_HPP

#include <json/json.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

#include <string>

class JsonConfiguration {
public:
    JsonConfiguration(const JsonConfiguration& other) = delete;
    void operator=(const JsonConfiguration& other) = delete;

    static JsonConfiguration *GetInstance();

    int Open(std::string pathname);
    void Close();

    Json::Value Read();
    int Write(const Json::Value& data);

    std::string GetPathname();

private:

    JsonConfiguration();

    static JsonConfiguration *json_configuration_;
    static pthread_mutex_t json_configured_mutex_;
    pthread_mutex_t modifying_mutex_;
    Json::Value json_configuration_data_;
    std::string configuration_pathname_;
    int json_configured_fd_;
    bool is_open_;

    struct timespec modified_time_;
    Json::StreamWriterBuilder writer_builder_;
    
    int Update();
};

#endif //LIBRARIES_JSON_CONFIGURATION_HPP