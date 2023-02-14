//
// Created by TrungTQ on 24/08/17.
//

#ifndef CORE_LINUX_H_
#define CORE_LINUX_H_

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "Libraries/Utils/Typedefs.h"

#define EINTR_LOOP(var, cmd)                \
    do {                                    \
        var = cmd;                          \
    } while (var == -1 || errno == EINTR)

static inline int_t
safe_open(
    const_char_p pPathName,
    int_t iFlags,
    mode_t mode = 0777
) {
    int_t iFd;
    EINTR_LOOP(iFd, ::open(pPathName, iFlags, mode));
    if (iFd != -1) {
        ::fcntl(iFd, F_SETFD, FD_CLOEXEC);
    }
    return iFd;
}

static inline int_t
safe_close(
    int_t iFd
) {
    int_t iRet = 0;
    EINTR_LOOP(iRet, ::close(iFd));
    return iRet;
}

static inline int_t
safe_read(
    int_t iFd,
    void_p pData,
    int_t iMaxLen
) {
    int_t iRet = 0;
    EINTR_LOOP(iRet, ::read(iFd, pData, iMaxLen));
    return iRet;
}

static inline int_t
safe_write(
    int_t iFd,
    void_p pData,
    int_t iLen
) {
    int_t iRet = 0;
    EINTR_LOOP(iRet, ::write(iFd, pData, iLen));
    return iRet;
}

#endif /* CORE_LINUX_H_ */
