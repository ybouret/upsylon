//! \file

#ifndef Y_PTR_ARR_INCLUDED
#define Y_PTR_ARR_INCLUDED 1

#include "y/ptr/ptr.hpp"

namespace upsylon {

    template <typename ARRAY, template <class> class PTR>
    class arr_ptr : public PTR<ARRAY>
    {
    public:
        typedef PTR<ARRAY> ptr_type;
        typedef typename ARRAY::type type;
        typedef typename ARRAY::mutable_type type;
        typedef typename ARRAY::const_type type;


        inline arr_ptr( ARRAY *arr ) : ptr_type(arr) {}
        inline virtual ~arr_ptr() throw() {}



        
    };

}

#endif

