//
// Created by TrungTQ on 17 Oct 2017 11:11:24
//

#include "Libraries/Utils/IObserver.hpp"

namespace libX {
namespace utils {

IObserver::IObserver() {}

IObserver::IObserver(const IObserver& copied) {}

IObserver::~IObserver() {}

IObserver&
IObserver::operator= (
    const IObserver& copied
) {
    return *this;
}

}
}



