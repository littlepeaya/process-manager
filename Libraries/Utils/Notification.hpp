//
// Created by TrungTQ on 29/09/17.
//

#ifndef LIBX_UTILS_NOTIFICATION_HPP_
#define LIBX_UTILS_NOTIFICATION_HPP_

#include "Libraries/Utils/AutoPtr.hpp"
#include "Libraries/Utils/RefCountedObj.hpp"

namespace libX {
namespace utils {

class Notification : public RefCountedObj {
public:
    typedef AutoPtr<Notification> Ptr_t;
    Notification();
protected:
    virtual ~Notification();
};

typedef Notification  Notification_t;
typedef Notification* Notification_p;

}
}

#endif //LIB_NOTIFICATION_HPP_
