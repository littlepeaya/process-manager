//
// Created by TrungTQ on 17 Oct 2017 10:35:48
//

#ifndef LIBX_UTILS_IOBSERVER_HPP_
#define LIBX_UTILS_IOBSERVER_HPP_

#include "Libraries/Utils/Typedefs.h"

class Notification;

namespace libX {
namespace utils {

class IObserver {
public:
    IObserver();
    IObserver(const IObserver& copied);
    IObserver& operator= (const IObserver& copied);

    virtual ~IObserver();

    virtual void Notify(Notification* pNf) const = 0;
};

}
}

#endif /* LIBX_UTILS_IOBSERVER_HPP_ */
