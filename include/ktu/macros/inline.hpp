#pragma once
#include <type_traits>
#if defined(__clang__) || defined(__GNUC__)
    #define KTU_INLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
    #define KTU_INLINE inline __forceinline
#elif defined(__TI_COMPILER_VERSION__)
    #define KTU_INLINE _Pragma("FUNC_ALWAYS_INLINE") inline
#endif

#define KTU_INLINE_ALIAS(name, aliased_item) \
    KTU_INLINE std::remove_reference<decltype(aliased_item)>::type name() {return aliased_item;}

#define KTU_INLINE_REFERENCE(name, referenced_item) \
    KTU_INLINE decltype(referenced_item) &name() {return referenced_item;}
