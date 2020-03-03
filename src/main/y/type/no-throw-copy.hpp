
//! \file
#ifndef Y_NO_THROW_COPY_INCLUDED
#define Y_NO_THROW_COPY_INCLUDED 1

#include "y/type/traits.hpp"


namespace upsylon {

    //! can we use memcpy/memmove...?
    template <typename T>
    struct no_throw_copy
    {
        enum { value = type_traits<T>::is_primitive };
    };

}

//! mark class as supporting no-throw-copy
#define Y_NO_THROW_COPY(CLASS) \
namespace upsylon {            \
template <>                    \
struct no_throw_copy< CLASS >  \
{ enum {value = true}; };      \
}

#endif
