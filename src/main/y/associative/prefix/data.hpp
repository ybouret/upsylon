//! \file

#ifndef Y_PREFIX_DATA_INCLUDED
#define Y_PREFIX_DATA_INCLUDED 1

#include "y/associative/prefix/defs.hpp"
#include "y/type/ints.hpp"

namespace upsylon {

    template <typename T>
    class prefix_data
    {
    public:
        typedef unsigned_int<sizeof(void*)>::type flag_t;

        inline explicit prefix_data() throw()
        {
            used = 1;
            assert(NULL!=addr);
        }

        inline explicit prefix_data(T *p) throw()
        {
            addr = p;
            assert(addr!=NULL);
            assert(used!=0);
        }

        virtual ~prefix_data() throw()
        {
            used = 0;
            assert(NULL==addr);
        }

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
