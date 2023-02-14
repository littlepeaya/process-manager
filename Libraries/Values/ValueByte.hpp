/// Created by TrungTQ on 20 Jan 2017 10:40:07

#ifndef LIBX_VAL_VALUEBYTE_HPP
#define LIBX_VAL_VALUEBYTE_HPP

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Values/Value.hpp"
#include "Libraries/Values/Enums/TypeValues.hpp"

namespace libX {
namespace val {

class ValueByte : public Value {
private:
    u8_t m_byValue;
protected:
    void Swap(ValueByte& other);
public:
    enum { Id = (u8_t) TypeValues::type_byte };

    enum {
        MIN = 0,
        MAX = 255
    };
    ValueByte(u8_t byValue = 0);
    ValueByte(const ValueByte& copied);

    virtual ~ValueByte();

    ValueByte& operator= (u8_t byValue);
    ValueByte& operator= (const ValueByte& copied);

    bool_t operator!= (const ValueByte& rhs) const;
    bool_t operator== (const ValueByte& rhs) const;
    bool_t operator>  (const ValueByte& rhs) const;
    bool_t operator>= (const ValueByte& rhs) const;
    bool_t operator<  (const ValueByte& rhs) const;
    bool_t operator<= (const ValueByte& rhs) const;

    bool_t operator!= (const u8_t byValue) const;
    bool_t operator== (const u8_t byValue) const;
    bool_t operator>  (const u8_t byValue) const;
    bool_t operator>= (const u8_t byValue) const;
    bool_t operator<  (const u8_t byValue) const;
    bool_t operator<= (const u8_t byValue) const;

    friend bool_t operator!= (const u8_t lhs, const ValueByte& rhs);
    friend bool_t operator== (const u8_t lhs, const ValueByte& rhs);
    friend bool_t operator>  (const u8_t lhs, const ValueByte& rhs);
    friend bool_t operator>= (const u8_t lhs, const ValueByte& rhs);
    friend bool_t operator<  (const u8_t lhs, const ValueByte& rhs);
    friend bool_t operator<= (const u8_t lhs, const ValueByte& rhs);

    operator u8_t() const;
};

typedef ValueByte  ValueByte_t;
typedef ValueByte* ValueByte_p;

}
}

#endif  /* !LIBX_VAL_VALUEBYTE_HPP */

