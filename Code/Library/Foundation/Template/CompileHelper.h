// CompileHelper.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_TEMPLATE_COMPILE_HELPER_H
#define FOUNDATION_TEMPLATE_COMPILE_HELPER_H


#define GENERATE_HAS_MEMBER(member) \
                                                                                  \
template<class T>																  \
class _HasMember_##member                                                         \
{                                                                                 \
private:                                                                          \
    using Yes = char[2];                                                          \
    using No  = char[1];                                                          \
                                                                                  \
    struct Fallback { int member; };                                              \
    struct Derived : T, Fallback { };                                             \
                                                                                  \
    template<class U>                                                             \
    static No& test(decltype(U::member)*);                                        \
    template<typename U>                                                          \
    static Yes& test(U*);                                                         \
                                                                                  \
public:                                                                           \
    static constexpr bool RESULT = sizeof(test<Derived>(nullptr)) == sizeof(Yes); \
};                                                                                \
                                                                                  \
template<>                                                                        \
class _HasMember_##member<void>                                                   \
{                                                                                 \
public:                                                                           \
	static constexpr bool RESULT = false;                                         \
};                                                                                \
                                                                                  \
template<class T>                                                                 \
struct HasMember_##member														  \
: public std::integral_constant<bool, _HasMember_##member<T>::RESULT> {};


//------------------------------------------------------------------------------
#endif // FOUNDATION_TEMPLATE_COMPILE_HELPER_H