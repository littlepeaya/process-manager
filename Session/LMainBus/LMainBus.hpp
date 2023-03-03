#ifndef SESSIONS_L_MAIN_BUS_HPP
#define SESSIONS_L_MAIN_BUS_HPP

#include "Libraries/LBus/LBus.hpp"

class LMainBus {
public:
    LMainBus(const LMainBus &other) = delete;

    void operator = (const LMainBus &other) = delete;

    static LBus *GetInstance();

protected:
    static LBus *main_bus_;

    LMainBus() = default;
};

#endif //SESSION_L_MAIN_BUS_HPP
