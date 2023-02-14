/// Created by TrungTQ on 20 Jan 2017 10:40:07

#ifndef LIBX_VAL_VALUE_HPP_
#define LIBX_VAL_VALUE_HPP_

#include <assert.h>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Values/Enums/TypeValues.hpp"

namespace libX {
namespace val {

class Value {
protected:
    u8_t m_byValueType;
    void Swap(Value& other);
public:
    Value(u8_t valueType = (u8_t) TypeValues::type_none);
    Value(const Value& copied);

    virtual ~Value();

    Value& operator= (const Value& copied);

    virtual u8_t GetType() const;
    virtual void SetType(u8_t byValueType);

    virtual bool_t operator== (const Value& rhs) const;
    virtual bool_t operator!= (const Value& rhs) const;
    virtual bool_t operator>= (const Value& rhs) const;
    virtual bool_t operator<= (const Value& rhs) const;
    virtual bool_t operator>  (const Value& rhs) const;
    virtual bool_t operator<  (const Value& rhs) const;
};

typedef Value  Value_t;
typedef Value* Value_p;

#define VALUE_ASSERT(x)     assert(x)

}
}

#endif  /* LIBX_VAL_VALUE_HPP_ */
