#ifndef LIBX_UTILS_NONCOPYABLE_HPP_
#define LIBX_UTILS_NONCOPYABLE_HPP_

namespace libX {
namespace utils {

class NonCopyable  {
public:
    NonCopyable() {}
    virtual ~NonCopyable() {}
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

}
}

#endif  /*LIBX_UTILS_NONCOPYABLE_HPP_*/
