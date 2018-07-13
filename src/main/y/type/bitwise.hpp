//! \file
#ifndef Y_TYPE_BITWISE_INCLUDED
#define Y_TYPE_BITWISE_INCLUDED 1

#include "y/type/args.hpp"


namespace upsylon {

    //! can we use memcpy/memmove...?
    template <typename T>
    struct support_bitwise_ops
    {
        enum { value = type_traits<T>::is_primitive };
    };

    //! do we need to destruct the type ?
    template <typename T>
    struct support_no_destruct
    {
        enum { value = type_traits<T>::is_primitive };
    };


    //! if we can use bitwise ops and no destruct.
    template <typename T>
    struct support_c_style_ops
    {
        enum { value = support_bitwise_ops<T>::value && support_no_destruct<T>::value };
    };

    //! mark class as supporting bitwise ops
#define Y_SUPPORT_BITWISE_OPS(CLASS) \
namespace upsylon {                        \
template <>                          \
struct support_bitwise_ops< CLASS >  \
{ enum {value = true}; };            \
}

    //! mark class as supporting no destruction
#define Y_SUPPORT_NO_DESTRUCT(CLASS) \
namespace upsylon {                        \
template <>                          \
struct support_no_destruct< CLASS >    \
{ enum {value = true}; };            \
}

    //! mark class as supporting C-style operations: bitwise and no destruction
#define YOCTO_SUPPORT_C_STYLE_OPS(CLASS) \
YOCTO_SUPPORT_BITWISE_OPS(CLASS) \
YOCTO_SUPPORT_NO_DESTRUCT(CLASS)

    
}

#endif

