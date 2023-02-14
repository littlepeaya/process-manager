/// Created by TrungTQ on 20 Jan 2017 10:40:07

#ifndef LIBX_VAL_VALUEINTEGER_HPP
#define LIBX_VAL_VALUEINTEGER_HPP

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Values/Value.hpp"
#include "Libraries/Values/Enums/TypeValues.hpp"

namespace libX {
namespace val {

class ValueInt : public Value {
private:
    int_t m_iValue;
protected:
    void Swap(ValueInt& other);
public:
    enum { Id = (u8_t) TypeValues::type_interger };
    ValueInt(int_t iValue = 0);
    ValueInt(const ValueInt& copied);

    virtual ~ValueInt();

    ValueInt& operator= (int_t iValue);
    ValueInt& operator= (ValueInt& copied);

    bool_t operator== (const ValueInt& rhs) const;
    bool_t operator!= (const ValueInt& rhs) const;
    bool_t operator>  (const ValueInt& rhs) const;
    bool_t operator>= (const ValueInt& rhs) const;
    bool_t operator<  (const ValueInt& rhs) const;
    bool_t operator<= (const ValueInt& rhs) const;

    bool_t operator== (const int_t iValue) const;
    bool_t operator!= (const int_t iValue) const;
    bool_t operator>  (const int_t iValue) const;
    bool_t operator>= (const int_t iValue) const;
    bool_t operator<  (const int_t iValue) const;
    bool_t operator<= (const int_t iValue) const;

    friend bool_t operator!= (const int_t lhs, const ValueInt& rhs);
    friend bool_t operator== (const int_t lhs, const ValueInt& rhs);
    friend bool_t operator>  (const int_t lhs, const ValueInt& rhs);
    friend bool_t operator>= (const int_t lhs, const ValueInt& rhs);
    friend bool_t operator<  (const int_t lhs, const ValueInt& rhs);
    friend bool_t operator<= (const int_t lhs, const ValueInt& rhs);

    operator int_t() const;
};

typedef ValueInt  ValueInt_t;
typedef ValueInt* ValueInt_p;

}
}

#endif  /* !LIBX_VAL_VALUEINTEGER_HPP */

