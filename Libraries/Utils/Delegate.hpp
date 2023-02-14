/// Created by TrungTQ on 27 Feb 2017 14:38:25

#ifndef LIBX_SINGNAL_DELEGATE_HPP_
#define LIBX_SINGNAL_DELEGATE_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Function.hpp"

namespace libX {
namespace utils {

template<typename RT, typename... Args>
struct IDelegate {
    IDelegate();
    virtual ~IDelegate();
    virtual RT operator()(Args... args) const = 0;
    virtual bool_t operator== (const IDelegate<RT, Args...>& copied) const = 0;
};

/// @fn     IDelegate
/// @brief  None
/// @param  None
/// @retval None
template<typename RT, typename... Args>
inline
IDelegate<RT, Args...>::IDelegate(
) {
}

/// @fn     ~IDelegate
/// @brief  None
/// @param  None
/// @retval None
template<typename RT, typename... Args>
inline
IDelegate<RT, Args...>::~IDelegate(
) {
}

template<typename RT, typename... Args>
struct FunctionDelegate : IDelegate<RT, Args...> {
    Function<RT(Args...)> m_Func;

    FunctionDelegate(Function<RT(Args...)> func);
    virtual ~FunctionDelegate();

    virtual RT operator()(Args... args) const;
    virtual bool_t operator== (const IDelegate<RT, Args...>& rhs) const;
};

/// @fn     Delegate
/// @brief  None
/// @param  None
/// @retval None
template<typename RT, typename... Args>
inline
FunctionDelegate<RT, Args...>::FunctionDelegate(
    Function<RT(Args...)> func
) : m_Func (func) {
}

/// @fn     ~Delegate
/// @brief  None
/// @param  None
/// @retval None
template<typename RT, typename... Args>
inline
FunctionDelegate<RT, Args...>::~FunctionDelegate(
) {
}

/// @fn     operator()
/// @brief  None
/// @param  None
/// @retval None
template<typename RT, typename... Args>
inline RT
FunctionDelegate<RT, Args...>::operator()(
    Args... args
) const {
    return m_Func(std::forward<Args>(args)...);
}

/// @fn     operator==
/// @brief  None
/// @param  None
/// @retval None
template<typename RT, typename... Args>
inline bool_t
FunctionDelegate<RT, Args...>::operator== (
    const IDelegate<RT, Args...>& rhs
) const {
    return FALSE;
}

template<class Class, typename RT, typename... Args>
struct ClassDelegate : IDelegate<RT, Args...> {
    Class* m_pInstance;
    RT (Class::*m_meThod)(Args...);

    ClassDelegate(Class* pInstance, RT (Class::*method)(Args...));
    virtual ~ClassDelegate();

    virtual RT operator()(Args... args) const;

    virtual bool_t operator== (const IDelegate<RT, Args...>& rhs) const;
};

/// @fn     ClassDelegate
/// @brief  None
/// @param  None
/// @retval None
template<class Class, typename RT, typename... Args>
inline
ClassDelegate<Class, RT, Args...>::ClassDelegate(
    Class* pInstance,
    RT (Class::*method)(Args...)
) : m_pInstance (pInstance)
  , m_meThod (method) {
}

/// @fn     ~ClassDelegate
/// @brief  None
/// @param  None
/// @retval None
template<class Class, typename RT, typename... Args>
inline
ClassDelegate<Class, RT, Args...>::~ClassDelegate(
) {
}

template<class Class, typename RT, typename... Args>
inline RT
ClassDelegate<Class, RT, Args...>::operator()(
    Args... args
) const {
    return (m_pInstance->*m_meThod)(std::forward<Args>(args)...);
}

/// @fn     operator==
/// @brief  None
/// @param  None
/// @retval None
template<class Class, typename RT, typename... Args>
inline bool_t
ClassDelegate<Class, RT, Args...>::operator== (
    const IDelegate<RT, Args...>& rhs
) const {
    auto other = dynamic_cast<const ClassDelegate*>(&rhs);
    return ((other != NULL) &&
            (other->m_pInstance == this->m_pInstance) &&
            (other->m_meThod == this->m_meThod));
}

template<class Class, typename RT, typename... Args>
struct ConstClassDelegate : IDelegate<RT, Args...> {
    Class* m_pInstance;
    RT (Class::*m_meThod)(Args...);

    ConstClassDelegate(Class* pInstance, RT (Class::*method)(Args...) const);
    virtual ~ConstClassDelegate();

    virtual RT operator()(Args... args) const;
    virtual bool_t operator== (const IDelegate<RT, Args...>& rhs) const;
};

/// @fn     ConstClassDelegate
/// @brief  None
/// @param  None
/// @retval None
template<class Class, typename RT, typename... Args>
inline
ConstClassDelegate<Class, RT, Args...>::ConstClassDelegate(
    Class* pInstance,
    RT (Class::*method)(Args...) const
) : m_pInstance (pInstance)
  , m_meThod (method) {

}

/// @fn     ~ConstClassDelegate
/// @brief  None
/// @param  None
/// @retval None
template<class Class, typename RT, typename... Args>
inline
ConstClassDelegate<Class, RT, Args...>::~ConstClassDelegate(
) {
}

/// @fn     operator()
/// @brief  None
/// @param  None
/// @retval None
template<class Class, typename RT, typename... Args>
inline RT
ConstClassDelegate<Class, RT, Args...>::operator()(
    Args... args
) const {
    return (m_pInstance->*m_meThod)(std::forward<Args>(args)...);
}

/// @fn     operator==
/// @brief  None
/// @param  None
/// @retval None
template<class Class, typename RT, typename... Args>
inline bool_t
ConstClassDelegate<Class, RT, Args...>::operator== (
    const IDelegate<RT, Args...>& rhs
) const {
    auto other = dynamic_cast<const ConstClassDelegate*>(&rhs);
    return ((other != NULL) &&
            (other->m_pInstance == this->m_pInstance) &&
            (other->m_meThod == this->m_meThod));
}

template <class Class, typename RT, class PT, class IT>
struct PolymorphicDelegate : IDelegate<RT, IT&> {
    Class* m_pInstance;
    RT (Class::*m_meThod)(PT&);

    PolymorphicDelegate(Class* pInstance, RT (Class::*method)(PT&) const);
    virtual ~PolymorphicDelegate();

    virtual RT operator()(IT& object) const;
    virtual bool_t operator== (const IDelegate<RT, IT&>& rhs) const;
};

template <class Class, typename RT, class PT, class IT>
inline
PolymorphicDelegate<Class, RT, PT, IT>::PolymorphicDelegate(
    Class* pInstance,
    RT (Class::*method)(PT&) const
) : m_pInstance (pInstance)
  , m_meThod (method) {

}

template <class Class, typename RT, class PT, class IT>
inline
PolymorphicDelegate<Class, RT, PT, IT>::~PolymorphicDelegate(
) {
}

template <class Class, typename RT, class PT, class IT>
inline RT
PolymorphicDelegate<Class, RT, PT, IT>::operator()(
    IT& object
) const {
    auto test = dynamic_cast<PT*>(&object);
    if (test != NULL) {
        return (m_pInstance->*m_meThod)(*test);
    }
    return RT();
}

template <class Class, typename RT, class PT, class IT>
inline bool_t
PolymorphicDelegate<Class, RT, PT, IT>::operator== (
    const IDelegate<RT, IT&>& rhs
) const {
    auto other = dynamic_cast<const PolymorphicDelegate*>(&rhs);
    return ((other != NULL) &&
            (other->m_pInstance == this->m_pInstance) &&
            (other->m_meThod == this->m_meThod));
}

}
}

#endif /* !LIBX_SINGNAL_DELEGATE_HPP_ */
