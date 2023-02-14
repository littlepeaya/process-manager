//
// Created by TrungTQ on 6 Jan 2017 20:48:25
//

#ifndef LIBX_UTILS_EXCEPTION_HPP_
#define LIBX_UTILS_EXCEPTION_HPP_

#include <stdarg.h>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"

namespace libX {
namespace utils {

class Exception : public std::exception {
public:
    Exception() noexcept;
    Exception(const Exception& copied) noexcept;
    Exception& operator= (const Exception& copied) noexcept;
    Exception(const_char_p message, ...) noexcept;
    virtual ~Exception() noexcept;

protected:
    String m_strWhat;
    void SetParams(const_char_p message, va_list args);

public:
    virtual const_char_p what() const noexcept;
};

typedef Exception  Exception_t;
typedef Exception* Exception_p;

class ExceptionInvalidCmd : public Exception {
public:
    ExceptionInvalidCmd() noexcept;
    ExceptionInvalidCmd(const ExceptionInvalidCmd& copied) noexcept;
    ExceptionInvalidCmd& operator= (const ExceptionInvalidCmd& copied) noexcept;
    ExceptionInvalidCmd(const_char_p message, ...) noexcept;
    virtual ~ExceptionInvalidCmd() noexcept;
    virtual const_char_p what() const noexcept;
};

typedef ExceptionInvalidCmd  ExceptionInvalidCmd_t;
typedef ExceptionInvalidCmd* ExceptionInvalidCmd_p;

class ExceptionNullPtr : public Exception {
public:
    ExceptionNullPtr() noexcept;
    ExceptionNullPtr(const ExceptionNullPtr& copied) noexcept;
    ExceptionNullPtr& operator= (const ExceptionNullPtr& copied) noexcept;
    ExceptionNullPtr(const_char_p message, ...) noexcept;
    virtual ~ExceptionNullPtr() noexcept;
    virtual const_char_p what() const noexcept;
};

typedef ExceptionNullPtr  ExceptionNullPtr_t;
typedef ExceptionNullPtr* ExceptionNullPtr_p;

class ExceptionErrDatabase : public Exception {
public:
    ExceptionErrDatabase() noexcept;
    ExceptionErrDatabase(const ExceptionErrDatabase& copied) noexcept;
    ExceptionErrDatabase& operator= (const ExceptionErrDatabase& copied) noexcept;
    ExceptionErrDatabase(const_char_p message, ...) noexcept;
    virtual ~ExceptionErrDatabase() noexcept;
    virtual const_char_p what() const noexcept;
};

class ExceptionInvalidArg : public Exception {
public:
    ExceptionInvalidArg() noexcept;
    ExceptionInvalidArg(const ExceptionInvalidArg& copied) noexcept;
    ExceptionInvalidArg& operator= (const ExceptionInvalidArg& copied) noexcept;
    ExceptionInvalidArg(const_char_p message, ...) noexcept;
    virtual ~ExceptionInvalidArg() noexcept;
    virtual const_char_p what() const noexcept;
};

typedef ExceptionInvalidArg  ExceptionInvalidArg_t;
typedef ExceptionInvalidArg* ExceptionInvalidArg_p;

}
}

#define THROW_EXCEPTION() \
    ST ( \
        throw Exception("not yet implement thread [%d] at [%s] line [%d]", pthread_self(), __FILENAME__, __LINE__); \
    )

#define THROW_MESSAGE(x, args...) \
    ST ( \
        throw Exception(x " thread [%d] at [%s] line [%d]", ##args, pthread_self(), __FILENAME__, __LINE__); \
    )

#define THROW_EXCEPTION_INVALIDCMD() \
    ST ( \
        throw ExceptionNullPtr("at [%s] line [%d]", pthread_self(), __FILENAME__, __LINE__); \
    )

#define THROW_MESSAGE_INVALIDCMD(x, args...) \
    ST ( \
        throw ExceptionNullPtr(x " thread [%d] at [%s] line [%d]", ##args, pthread_self(), __FILENAME__, __LINE__); \
    )

#define THROW_EXCEPTION_NULLPTR() \
    ST ( \
        throw ExceptionNullPtr("thread [%d] at [%s] line [%d]", pthread_self(), __FILENAME__, __LINE__); \
    )

#define THROW_MESSAGE_NULLPTR(x, args...) \
    ST ( \
        throw ExceptionNullPtr(x " thread [%d] at [%s] line [%d]", ##args, pthread_self(), __FILENAME__, __LINE__); \
    )

#define THROW_EXCEPTION_INVALIDARG() \
    ST ( \
        throw ExceptionInvalidArg("thread [%d] at [%s] line [%d]", pthread_self(), __FILENAME__, __LINE__); \
    )

#define THROW_MESSAGE_INVALIDARG(x, args...) \
    ST ( \
        throw ExceptionInvalidArg(x " thread [%d] at [%s] line [%d]", ##args, pthread_self(), __FILENAME__, __LINE__); \
    )

#endif /* !EXCEPTION_HPP_ */
