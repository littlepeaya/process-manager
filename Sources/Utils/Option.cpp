//
// Created by TrungTQ on 27 Nov 2017 22:00:17
//

#include "Libraries/Utils/Option.hpp"

namespace libX {
namespace utils {

String m_shortName;
String m_fullName;
String m_description;
bool_t m_required;
bool_t m_repeatable;
String m_argName;
bool_t m_argRequired;
String m_group;
String m_binding;

Option::Option(
) : m_required (FALSE)
  , m_repeatable (FALSE)
  , m_argRequired (FALSE) {}

Option::Option(
    const String& fullName,
    const String& shortName
) : m_shortName (shortName)
  , m_fullName (fullName)
  , m_required (FALSE)
  , m_repeatable (FALSE)
  , m_argRequired (FALSE) {}

Option::Option(
    const String& fullName,
    const String& shortName,
    const String& description,
    bool_t required
) : m_shortName (shortName)
  , m_fullName (fullName)
  , m_description (description)
  , m_required (required)
  , m_repeatable (FALSE)
  , m_argRequired (FALSE) {}

Option::Option(
    const String& fullName,
    const String& shortName,
    const String& description,
    bool_t required,
    const String& argName,
    bool_t argRequired
) : m_shortName (shortName)
  , m_fullName (fullName)
  , m_description (description)
  , m_required (required)
  , m_repeatable (FALSE)
  , m_argName (argName)
  , m_argRequired (argRequired) {}

Option::Option(
    const Option& copied
) : m_shortName (copied.m_shortName)
  , m_fullName (copied.m_fullName)
  , m_description (copied.m_description)
  , m_required (copied.m_required)
  , m_repeatable (copied.m_repeatable)
  , m_argName (copied.m_argName)
  , m_argRequired (copied.m_argRequired) {}

Option::~Option() {}

Option&
Option::operator= (
    const Option& copied
) {
    Option temp(copied);
    Swap(temp);
    return *this;
}

void
Option::Swap(
    Option& other
) {
    std::swap(m_shortName, other.m_shortName);
    std::swap(m_fullName, other.m_fullName);
    std::swap(m_description, other.m_description);
    std::swap(m_required, other.m_required);
    std::swap(m_repeatable, other.m_repeatable);
    std::swap(m_argName, other.m_argName);
    std::swap(m_argRequired, other.m_argRequired);
}

Option&
Option::ShortName(
    const String& shortName
) {
    m_shortName = shortName;
    return *this;
}

Option&
Option::FullName(
    const String& fullName
) {
    m_fullName = fullName;
    return *this;
}

Option&
Option::Description(
    const String& description
) {
    m_description = description;
    return *this;
}

Option&
Option::Required(
    bool_t required
) {
    m_required = required;
    return *this;
}

Option&
Option::Repeatable(
    bool_t flag
) {
    m_repeatable = flag;
    return *this;
}

Option&
Option::Argument(
    const String& argName,
    bool_t required
) {
    m_argName = argName;
    m_required = required;
    return *this;
}

Option&
Option::NoArgument(
) {
    m_argName.clear();
    m_argRequired = FALSE;
    return *this;
}

Option&
Option::Group(
   const String& group
) {
    m_group = group;
    return *this;
}

Option&
Option::Binding(
    const String& propertyName
) {}

// Option& Binding(const String& propertyName, )

const String&
Option::ShortName(
) const {
    return m_shortName;
}

const String&
Option::FullName(
) const {
    return m_fullName;
}

const String&
Option::Description(
) const {
    return m_description;
}

bool_t
Option::Required(
) const {
    return m_required;
}

bool_t
Option::Repeatable(
) const {
    return m_repeatable;
}

bool_t
Option::ArgRequired(
) const {
    return m_argRequired;
}

const String&
Option::ArgName(
) const {
    return m_argName;
}

const String&
Option::Group(
) const {
    return m_group;
}

const String&
Option::Binding(
) const {
    return m_binding;
}

}
}
