//! \file

#ifndef Y_PREFIX_DATA_INCLUDED
#define Y_PREFIX_DATA_INCLUDED 1

#include "y/associative/prefix/defs.hpp"
#include "y/type/ints.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! variant data for nodes
    //
    //__________________________________________________________________________
    template <typename T>
    class prefix_data
    {
    public:
        //! unsigned integral matching addresses
        typedef unsigned_int<sizeof(void*)>::type flag_t;

        //! set 'used=false'
        inline explicit prefix_data() throw() { used = 0; }

        //! set 'addr=ptr'
        inline explicit prefix_data(T *ptr) throw()
        {
            addr = ptr;
        }

        //! cleanup
        virtual ~prefix_data() throw()
        {
            used = 0;
            assert(NULL==addr);
        }

        //! the data
        union
        {
            T      *addr;
            flag_t  used;
        };

    private:
        Y_DISABLE_COPY_AND_ASSIGN(prefix_data);
    };
    
}

#endif
