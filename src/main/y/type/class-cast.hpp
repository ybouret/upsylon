//! \file

#ifndef Y_TYPE_CLASS_CAST_INCLUDED
#define Y_TYPE_CLASS_CAST_INCLUDED 1

#include "y/type/class-conversion.hpp"
#include "y/type/select.hpp"

namespace upsylon {

    template <typename T,typename U>
    class class_cast
    {
    public:
        static const bool derived = Y_IS_SUPERSUBCLASS(T,U);
        typedef typename select_type<derived,T &,      T>::result type;
        typedef typename select_type<derived,const T &,T>::result const_type;

        static inline type       _( U       &args ) { return __(args,int2type<derived>()); }
        static inline const_type _( const U &args ) { return __(args,int2type<derived>()); }

    private:
        
        static inline T & __( U &args, int2type<true>  ) throw() { return args; }
        static inline T   __( U &args, int2type<false> )         { return static_cast<T>(args); }
        
        static inline const T & __( const U &args, int2type<true>  ) throw() { return args; }
        static inline T         __( const U &args, int2type<false> )         { return static_cast<T>(args); }

        
    };

}


#endif
