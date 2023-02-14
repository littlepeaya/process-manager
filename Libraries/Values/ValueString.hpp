/// Created by TrungTQ on 20 Jan 2017 10:40:07

#ifndef LIBX_VAL_VALUESTRING_HPP
#define	LIBX_VAL_VALUESTRING_HPP

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"
#include "Libraries/Values/Value.hpp"
#include "Libraries/Values/Enums/TypeValues.hpp"

namespace libX {
namespace val {

class ValueString : public Value {
private:
    String  m_strValue;
protected:
    void Swap(ValueString& other);
public:
    enum { Id = (u8_t) TypeValues::type_string };
    ValueString(const String& strValue = String());
    ValueString(const ValueString& other);

    virtual ~ValueString();

    ValueString& operator= (String strValue);
    ValueString& operator= (const ValueString& copied);

    void Push_Back(char ch);
    void Pop_Back();
    void Clear();

    size_t Size()  const;
    bool_t Empty() const;

    bool_t operator== (const ValueString& rhs) const;
    bool_t operator!= (const ValueString& rhs) const;
    bool_t operator>  (const ValueString& rhs) const;
    bool_t operator>= (const ValueString& rhs) const;
    bool_t operator<  (const ValueString& rhs) const;
    bool_t operator<= (const ValueString& rhs) const;

    bool_t operator== (const String& strValue) const;
    bool_t operator!= (const String& strValue) const;
    bool_t operator>  (const String& strValue) const;
    bool_t operator>= (const String& strValue) const;
    bool_t operator<  (const String& strValue) const;
    bool_t operator<= (const String& strValue) const;

    operator String() const;
};

typedef ValueString  ValueString_t;
typedef ValueString* ValueString_p;

}
}

#endif	/* LIBX_VAL_VALUESTRING_HPP */

