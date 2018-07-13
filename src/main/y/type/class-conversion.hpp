//! \file
#ifndef Y_CLASS_CONVERSION_INCLUDED
#define Y_CLASS_CONVERSION_INCLUDE 1

#include "y/os/platform.hpp"

namespace upsylon
{
    template <class T,class U>
    class conversion {
    private:
        typedef uint8_t small_type;
        class           large_type { uint32_t unused; };

        static  small_type test( U );
        static  large_type test(...);
        static  T     makeT();

    public:
        conversion(); //!< or some compiler will complain about all private...
        enum
        {
            exists   = ( sizeof( test(makeT()) ) == sizeof(small_type) ),
            same_type = false
        };
    };

    template <class T >
    class conversion<T,T> {
    public:
        enum {
            exists   = true,
            same_type = true
        };
    };

#define Y_IS_SUPERSUBCLASS(T,U) \
(conversion<const U *,const T *>::exists && (!conversion<const T *, const void *>::same_type) )

#define Y_IS_SUPERSUBCLASS_STRICT(T,U) \
( YOCTO_IS_SUPERSUBCLASS(T,U) && (!conversion<const T,const U>::same_type) )

}


#endif
