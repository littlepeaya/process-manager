/// Created by TrungTQ on 20 Jan 2017 10:40:07

#ifndef LIBX_VAL_VALUEDWORD_HPP_
#define LIBX_VAL_VALUEDWORD_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Values/Value.hpp"
#include "Libraries/Values/Enums/TypeValues.hpp"

namespace libX {
namespace val {

class ValueDword : public Value {
private:
    u32_t m_dwValue;
protected:
    void Swap(ValueDword& other);
public:
    enum { Id = (u8_t) TypeValues::type_dword };

    ValueDword(u32_t dwValue = 0);
    ValueDword(const ValueDword& copied);

    virtual ~ValueDword();

    ValueDword& operator= (u32_t dwValue);
    ValueDword& operator= (ValueDword& copied);

    bool_t operator!= (const ValueDword& rhs) const;
    bool_t operator== (const ValueDword& rhs) const;
    bool_t operator>  (const ValueDword& rhs) const;
    bool_t operator>= (const ValueDword& rhs) const;
    bool_t operator<  (const ValueDword& rhs) const;
    bool_t operator<= (const ValueDword& rhs) const;

    bool_t operator!= (const u32_t dwValue) const;
    bool_t operator== (const u32_t dwValue) const;
    bool_t operator>  (const u32_t dwValue) const;
    bool_t operator>= (const u32_t dwValue) const;
    bool_t operator<  (const u32_t dwValue) const;
    bool_t operator<= (const u32_t dwValue) const;

    friend bool_t operator!= (const u32_t lhs, const ValueDword& rhs);
    friend bool_t operator== (const u32_t lhs, const ValueDword& rhs);
    friend bool_t operator>  (const u32_t lhs, const ValueDword& rhs);
    friend bool_t operator>= (const u32_t lhs, const ValueDword& rhs);
    friend bool_t operator<  (const u32_t lhs, const ValueDword& rhs);
    friend bool_t operator<= (const u32_t lhs, const ValueDword& rhs);

    operator u32_t() const;
};

typedef ValueDword  ValueDword_t;
typedef ValueDword* ValueDword_p;

}
}

#endif /* !LIBX_VAL_VALUEDWORD_HPP_ */
