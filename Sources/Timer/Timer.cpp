#include "Libraries/Timer/Timer.hpp"

#include "Libraries/Log/LogPlus.hpp"

Timer::Timer() :
    timerid_(0),
    sigev_{0},
    has_started_(false) {
        
    sigev_.sigev_notify = SIGEV_THREAD;
    sigev_.sigev_notify_function = HandleTimerHandlers;
    sigev_.sigev_value.sival_ptr = this;
}

Timer::~Timer() {
    if (has_started_)
        timer_delete(timerid_);

    while (!timer_handler_vector_.empty()) {
        delete timer_handler_vector_.back();
        timer_handler_vector_.pop_back();
    }
}

int Timer::Start(time_t start_point, time_t interval) {
    if (start_point == 0) {
        LOG_ERRO("the start_point variable is not equal 0");
        return -1;
    }
    
    if (!has_started_) {
        if (timer_create(CLOCK_REALTIME, &sigev_, &timerid_) < 0) {
            LOG_ERRO("Failed to timer_create, err=%s", strerror(errno));
            return -1;
        }

        has_started_ = true;
    }
    //hen gio dinh ki het han sau moi start_point 
    struct itimerspec itspec = {0};
    itspec.it_value.tv_sec = start_point / 1000;
    itspec.it_value.tv_nsec = (start_point % 1000) * 1000000;
    itspec.it_interval.tv_sec = interval / 1000;
    itspec.it_interval.tv_nsec = (interval % 1000) * 1000000;
    //khoi tao bo dem thoi gian bat dau 
    if (timer_settime(timerid_, 0, &itspec, nullptr) < 0) {
        LOG_ERRO("Failed to timer_settime, err=%s", strerror(errno));
        return -1;
    }

    return 0;
}

int Timer::Stop() {
    struct itimerspec itspec = {0};

    if (timer_settime(timerid_, 0, &itspec, nullptr) < 0) {
        LOG_ERRO("Failed to timer_settime, err=%s", strerror(errno));
        return -1;
    }
}

void Timer::HandleTimerHandlers(union sigval arg) {
    auto *obj = (Timer *)arg.sival_ptr;

    for (auto it = obj->timer_handler_vector_.begin(); it != obj->timer_handler_vector_.end(); ++it)
        (*it)->routine((*it)->user_data); 
}

void Timer::RegisterTimerHandler(int (*routine)(void *), void *user_data) {
    TimerHandler *timer_handler;

    timer_handler = new TimerHandler;
    timer_handler->routine = routine;
    timer_handler->user_data = user_data;
    timer_handler_vector_.push_back(timer_handler);
}

void Timer::CancelTimerHandler(int (*routine)(void *user_data)) {
    void *user_data = nullptr;

    for (auto it = timer_handler_vector_.begin(); it != timer_handler_vector_.end(); ++it) {
        if ((*it)->routine == routine) {
            user_data = (*it)->user_data;
            delete *it;
            timer_handler_vector_.erase(it);
            break;
        }
    }
}
