//
// Created by TrungTQ on 09/08/17.
//

#ifndef LIBX_UTILS_COREIMPL_HPP_
#define LIBX_UTILS_COREIMPL_HPP_

#include "Libraries/Utils/Core.hpp"
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Atomic/AtomicCounter.hpp"

namespace libX {
namespace utils {

class CoreImpl {
private:
    static AtomicCounter m_counter;
public:
    /// @fn     CoreImpl
    /// @brief  Constructor
    /// @param  None
    /// @return None
    CoreImpl();

    /// @fn     ~CoreImpl
    /// @brief  Destructor
    /// @param  None
    /// @return None
    virtual ~CoreImpl();

    /// @fn     Index()
    /// @brief  Index of object
    /// @param  None
    /// @return IDWORD index of object
    i32_t Index() const;

    /// @fn     NbrObj
    /// @brief  None
    /// @param  None
    /// @return IDWORD number of object
    static i32_t NbrObj();

    Core* m_ptr;
};

}
}

#endif //LIBX_UTILS_COREIMPL_HPP_
