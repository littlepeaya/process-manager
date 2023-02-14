//
// Created by TrungTQ on 27 Nov 2017 21:38:08
//

#ifndef LIBX_UTILS_OPTION_HPP_
#define LIBX_UTILS_OPTION_HPP_

#include "Libraries/Utils/String.hpp"

namespace libX {
namespace utils {

class Option {
public:
    Option();

    Option(const Option& copied);

    Option(const String& fullName, const String& shortName);

    Option(const String& fullName, const String& shortName, const String& description, bool_t required = FALSE);

    Option(const String& fullName, const String& shortName, const String& description, bool_t required, const String& argName, bool_t argRequired = FALSE);

    ~Option();

    Option& operator= (const Option& copied);

    void Swap(Option& another);

    Option& ShortName(const String& shortName);

    Option& FullName(const String& fullName);

    Option& Description(const String& description);

    Option& Required(bool_t required);

    Option& Repeatable(bool_t flag);

    Option& Argument(const String& argName, bool_t required = TRUE);

    Option& NoArgument();

    Option& Group(const String& group);

    Option& Binding(const String&  propertyName);

    // Option& Binding(const String& propertyName, )

    const String& ShortName() const;

    const String& FullName() const;

    const String& Description() const;

    bool_t Required() const;

    bool_t Repeatable() const;

    bool_t ArgRequired() const;

    const String& ArgName() const;

    const String& Group() const;

    const String& Binding() const;

private:
    String m_shortName;
    String m_fullName;
    String m_description;
    bool_t m_required;
    bool_t m_repeatable;
    String m_argName;
    bool_t m_argRequired;
    String m_group;
    String m_binding;
};

}
}

#endif /* LIBX_UTILS_OPTION_HPP_ */
