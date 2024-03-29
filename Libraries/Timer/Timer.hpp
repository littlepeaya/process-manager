#ifndef LIBRARIES_TIMER_HPP
#define LIBRARIES_TIMER_HPP

#include <signal.h>
#include <time.h>

#include <string>
#include <vector>

typedef struct TimerHandler_t {
    int (*routine)(void *user_data);
    void *user_data;
} TimerHandler;

class Timer {
public:
    Timer();
    ~Timer();

    int Start(time_t start_point, time_t interval); // unit in a ms
    int Stop();

    void RegisterTimerHandler(int (*routine)(void *user_data), void *user_data);
    void CancelTimerHandler(int (*routine)(void *user_data));
    
private:
    timer_t timerid_;
    struct sigevent sigev_;
    bool has_started_;
    std::vector<TimerHandler *> timer_handler_vector_;

    static void HandleTimerHandlers(union sigval);
};

#endif //LIBRARIES_TIMER_HPP