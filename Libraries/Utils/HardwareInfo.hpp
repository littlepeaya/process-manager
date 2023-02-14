/// Created by TrungTQ on 21 Feb 2017 14:14:20

#ifndef LIBX_UTILS_HARDWAREINFO_HPP_
#define LIBX_UTILS_HARDWAREINFO_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"
#include "Locker.hpp"

#ifndef IF_DEFAULT
#ifdef MT7688
#define IF_DEFAULT "eth0.1"
#else
#define IF_DEFAULT "wlp3s0"
#endif
#endif

namespace libX {
    namespace utils {

        class HardwareInfo {
        public:
            static utils::String GetMacAddr(const utils::String& iface = IF_DEFAULT);
            static utils::String GetIpAddr();
            static utils::String GetNetMask(const utils::String& iface = IF_DEFAULT);
            static utils::String GetGatewayAddr(const utils::String& iface = IF_DEFAULT);
            static utils::String GetSSID() { return utils::String(); }

            static void SetRunPath(const String & run_path);
            static String GetRunPath();
            static void SetSourcePath(const String & source_path);
            static String GetSourcePath();
            static void SetProgramName(const String & source_path);
            static String GetProgramName();

        private:
            static String run_path_;
            static String source_path_;
            static String program_name_;
            static Locker_t locker_;
        };

    }
}

#endif /* !LIBX_UTILS_HARDWAREINFO_HPP_ */
