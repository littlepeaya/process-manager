/// Created by TrungTQ on 20 Jan 2017 10:40:07

#ifndef LIBX_VAL_VALUEBOOL_HPP_
#define LIBX_VAL_VALUEBOOL_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Values/Value.hpp"
#include "Libraries/Values/Enums/TypeValues.hpp"

namespace libX {
namespace val {

class ValueBool : public Value {
private:
    bool_t m_boValue;
protected:
    void Swap(ValueBool& other);
public:
    enum { Id = (u8_t) TypeValues::type_bool };

    ValueBool(bool_t boValue = FALSE);
    ValueBool(const ValueBool& copied);

    virtual ~ValueBool();

    ValueBool& operator= (bool_t boValue);
    ValueBool& operator= (const ValueBool& copied);

    bool_t operator== (const ValueBool& rhs) const;
    bool_t operator!= (const ValueBool& rhs) const;

    bool_t operator== (const bool_t boValue) const;
    bool_t operator!= (const bool_t boValue) const;

    friend bool_t operator!= (const bool_t lhs, const ValueBool& rhs);
    friend bool_t operator== (const bool_t lhs, const ValueBool& rhs);

    operator bool_t() const;
};

typedef ValueBool  ValueBool_t;
typedef ValueBool* ValueBool_p;

}
}

#endif /* !LIBX_VAL_VALUEBOOL_HPP_ */
