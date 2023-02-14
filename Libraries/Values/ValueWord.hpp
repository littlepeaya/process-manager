/// Created by TrungTQ on 20 Jan 2017 10:40:07

#ifndef LIBX_VAL_VALUEWORD_HPP_
#define LIBX_VAL_VALUEWORD_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Values/Value.hpp"
#include "Libraries/Values/Enums/TypeValues.hpp"

namespace libX {
namespace val {

class ValueWord : public Value {
private:
    enum { Id = (u8_t) TypeValues::type_word };
    u16_t m_wValue;
protected:
    void Swap(ValueWord& other);
public:
    ValueWord(u16_t wValue = 0);
    ValueWord(const ValueWord& copied);

    virtual ~ValueWord();

    ValueWord& operator= (u16_t wValue);
    ValueWord& operator= (ValueWord& copied);

    bool_t operator== (const ValueWord& rhs) const;
    bool_t operator!= (const ValueWord& rhs) const;
    bool_t operator>  (const ValueWord& rhs) const;
    bool_t operator>= (const ValueWord& rhs) const;
    bool_t operator<  (const ValueWord& rhs) const;
    bool_t operator<= (const ValueWord& rhs) const;

    bool_t operator== (const u16_t wValue) const;
    bool_t operator!= (const u16_t wValue) const;
    bool_t operator>  (const u16_t wValue) const;
    bool_t operator>= (const u16_t wValue) const;
    bool_t operator<  (const u16_t wValue) const;
    bool_t operator<= (const u16_t wValue) const;

    friend bool_t operator!= (const u16_t lhs, const ValueWord& rhs);
    friend bool_t operator== (const u16_t lhs, const ValueWord& rhs);
    friend bool_t operator>  (const u16_t lhs, const ValueWord& rhs);
    friend bool_t operator>= (const u16_t lhs, const ValueWord& rhs);
    friend bool_t operator<  (const u16_t lhs, const ValueWord& rhs);
    friend bool_t operator<= (const u16_t lhs, const ValueWord& rhs);

    operator u16_t() const;
};

typedef ValueWord  ValueWord_t;
typedef ValueWord* ValueWord_p;

}
}

#endif /* !LIBX_VAL_VALUEWORD_HPP_ */
