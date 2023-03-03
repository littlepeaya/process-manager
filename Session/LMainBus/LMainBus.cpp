#include "LMainBus.hpp"

#include "Libraries/LBus/LBus.hpp"

LBus *
LMainBus::main_bus_ = nullptr;

LBus *
LMainBus::GetInstance() {
    if (main_bus_ == nullptr) {
        main_bus_ = new LBus();
    }

    return main_bus_;
}