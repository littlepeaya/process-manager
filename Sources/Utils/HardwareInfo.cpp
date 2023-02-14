/// Created by TrungTQ on 27/10/17.

#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <Libraries/Utils/AutoLocker.hpp>
#include "Libraries/Utils/Converter.hpp"
#include "Libraries/Utils/HardwareInfo.hpp"
#include "Libraries/Log/LogPlus.hpp"

namespace libX {
    namespace utils {

        String HardwareInfo::run_path_;
        String HardwareInfo::source_path_;
        String HardwareInfo::program_name_;
        Locker_t HardwareInfo::locker_;

/// @func   GetMacAddr
/// @brief  None
/// @param  None
/// @retval None
        utils::String
        HardwareInfo::GetMacAddr(
                const utils::String &iface
        ) {
            int_t fd = socket(AF_INET, SOCK_DGRAM, 0);
            struct ifreq ifr;
            ifr.ifr_addr.sa_family = AF_INET;
            strncpy(ifr.ifr_name, iface.c_str(), IFNAMSIZ - 1);
            ioctl(fd, SIOCGIFHWADDR, &ifr);
            close(fd);
            utils::String macRet;
            bool_t boIsFirst = TRUE;
            for (u32_t i = 0; i < 6; i++) {
                if (!boIsFirst) {
                    macRet.append(":");
                }
                macRet.append(Converter::GetHex(ifr.ifr_hwaddr.sa_data[i]));
                boIsFirst = FALSE;
            }
            return macRet;
        }

/// @func   GetIpAddr
/// @brief  None
/// @param  None
/// @retval None
        utils::String
        HardwareInfo::GetIpAddr(
        ) {
            LOG_DBUG("GetLocalIP");
            String ip("0.0.0.0");
            FILE *file = fopen("/proc/net/route", "r");
            char line[100], *ifaName, *destination, host[NI_MAXHOST] = {0};
            struct ifaddrs *ifaddr, *ifa;
            int family, rc;

            while (fgets(line, 100, file)) {
                ifaName = strtok(line, " \t");
                destination = strtok(NULL, " \t");
                if (ifaName != NULL && destination != NULL) {
                    if (strcmp(destination, "00000000") == 0) {
                        LOG_DBUG("default iface: %s", ifaName);
                        break;
                    }
                }
            }

            if (getifaddrs(&ifaddr) == -1) {
                LOG_ERRO("Could not get local ip");
                return ip;
            }

            for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
                if (ifa->ifa_addr == NULL) continue;
                family = ifa->ifa_addr->sa_family;
                if (strcmp(ifa->ifa_name, ifaName) == 0) {
                    if (family == AF_INET) {
                        rc = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0,
                                         NI_NUMERICHOST);
                        if (rc != 0) {
                            LOG_ERRO("getnameinfo error");
                            return ip;
                        }
                        return String(host);
                    }
                }
            }

            return ip;
        }

        utils::String
        HardwareInfo::GetNetMask(
                const utils::String &iface
        ) {
            int_t fd = socket(AF_INET, SOCK_DGRAM, 0);
            struct ifreq ifr;
            ifr.ifr_addr.sa_family = AF_INET;
            strncpy(ifr.ifr_name, iface.c_str(), IFNAMSIZ - 1);
            ioctl(fd, SIOCGIFNETMASK, &ifr);
            close(fd);
            utils::String netmask(inet_ntoa(((struct sockaddr_in *) &ifr.ifr_netmask)->sin_addr));
            return netmask;
        }

/// @func   GetGatewayAddr
/// @brief  None
/// @param  None
/// @retval None
        utils::String
        HardwareInfo::GetGatewayAddr(
                const utils::String &iface
        ) {
            String gateway = "0.0.0.0";

            FILE *fp = popen("netstat -rn", "r");
            char line[256] = {0x0};

            while (fgets(line, sizeof(line), fp) != NULL) {
                /*
                 * Download destination.
                 */
                char *destination;
                destination = strndup(line, 15);

                String interface;
                interface = strndup(line + 73, 6);
                interface.erase(std::remove_if(interface.begin(), interface.end(), isspace), interface.end());
                // Find line with the gateway
                if (strcmp("0.0.0.0        ", destination) == 0 && strcmp(interface.c_str(), iface.c_str()) == 0) {
                    // Extract gateway
                    gateway = strndup(line + 16, 15);
                }
                free(destination);
                gateway.erase(std::remove_if(gateway.begin(), gateway.end(), isspace), gateway.end());
            }

            pclose(fp);
            return gateway;
        }

        libX::utils::String HardwareInfo::GetRunPath() {
            return run_path_;
        }

        libX::utils::String HardwareInfo::GetSourcePath() {
            return source_path_;
        }

        libX::utils::String HardwareInfo::GetProgramName() {
            return program_name_;
        }

        void HardwareInfo::SetRunPath(const libX::utils::String & run_path) {
            locker_.Lock();
            run_path_ = run_path;
            locker_.UnLock();
        }

        void HardwareInfo::SetSourcePath(const libX::utils::String & source_path) {
            locker_.Lock();
            source_path_ = source_path;
            locker_.UnLock();
        }

        void HardwareInfo::SetProgramName(const libX::utils::String & program_name) {
            locker_.Lock();
            program_name_ = program_name;
            locker_.UnLock();
        }
    }
}
