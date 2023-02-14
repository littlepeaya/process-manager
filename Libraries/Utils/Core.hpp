//
// Created by TrungTQ on 22 Jan 2017 07:36:46
//

#ifndef LIBX_UTILS_CORE_HPP_
#define LIBX_UTILS_CORE_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"
#include "Libraries/Utils/ScopedPtr.hpp"

namespace libX {
namespace utils {

class CoreImpl;

class Core {
protected:
    ScopedPtr<CoreImpl> m_ptrImpl;

private:
    /// @fn     PtrImpl
    /// @brief  None
    /// @param  None
    /// @return IMPL*
    CoreImpl* PtrImpl();

    /// @fn     PtrImpl
    /// @brief  None
    /// @param  None
    /// @return IMPL*
    const CoreImpl* PtrImpl() const;

    DISABLE_COPY(Core)
public:
    /// @fn     Core
    /// @brief  Constructor
    /// @param  None
    /// @return None
    Core();

    /// @fn     Core
    /// @brief  Constructor
    /// @param  COREIMPL&
    /// @return None
    explicit Core(CoreImpl& d);

    /// @fn     Core
    /// @brief  Constructor
    /// @param  COREIMPL* ptr
    /// @return None
    explicit Core(CoreImpl* ptr);

    /// @fn     ~Core
    /// @brief  Destructor
    /// @param  None
    /// @return None
    virtual ~Core();

    /// @fn     Index()
    /// @brief  Index of object
    /// @param  None
    /// @return IDWORD index of object
    i32_t Index() const;

    /// @fn     ObjName
    /// @brief  None
    /// @param  None
    /// @return STRING name of object
    virtual String ObjName() const;

    /// @fn     NbrObj
    /// @brief  None
    /// @param  None
    /// @return IDWORD number of object
    static i32_t NbrObj();
};

typedef Core  Core_t;
typedef Core* Core_p;

}
}

#endif /* CORE_HPP_ */
