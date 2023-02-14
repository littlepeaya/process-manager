/// Lumi, JSC. Created by TrungTQ on 22 Oct 2017 22:03:55.

#ifndef LIBX_UTILS_APPLICATION_HPP_
#define LIBX_UTILS_APPLICATION_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"

namespace libX {
namespace utils {

class Application {
public:
    /// @fn     Application
    /// @brief  Constructor
    /// @param  None
    /// @return None
    Application();

    /// @fn     ~Application
    /// @brief  Destructor
    /// @param  None
    /// @return None
    virtual ~Application() = default;

    /// @fn     PrintUsage
    /// @brief  None
    /// @param  None
    /// @return None
    virtual void PrintUsage() = 0;

    /// @fn     Run
    /// @brief  None
    /// @param  None
    /// @return INT
    virtual bool_t Run() = 0;

    /// @fn     LoadConfig
    /// @brief  None
    /// @param  STRING path
    /// @return BOOL
    virtual bool_t LoadConfig(const String& pathToConfig) = 0;

    /// @fn     GetArguments
    /// @brief  None
    /// @param  argc
    /// @param  argv
    /// @return BOOL
    virtual bool_t GetArguments(int argc, char* argv[]) = 0;

    /// @fn     Uninitialize
    /// @brief  None
    /// @param  None
    /// @return None
    virtual bool_t Uninitialize() = 0;
};

}
}

#endif /* LIBX_APP_APPLICATION_HPP_ */
