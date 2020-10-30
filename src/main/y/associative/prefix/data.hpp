//! \file

#ifndef Y_PREFIX_DATA_INCLUDED
#define Y_PREFIX_DATA_INCLUDED 1

#include "y/associative/prefix/defs.hpp"

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
        typedef prefix_::flag_t flag_t; //!< alias
        
        inline explicit prefix_data()       throw() { used = 0;   } //!< set 'used=0'
        inline explicit prefix_data(T *ptr) throw() { addr = ptr; } //!< set 'addr=ptr'
        virtual        ~prefix_data() throw() { used = 0; assert(NULL==addr); } //!M cleanup

        //! the data
        union
        {
            T      *addr;
            flag_t  used;
        };

        //! get a value to check in_use
        static inline T * in_use_addr() throw()
        {
            union { flag_t _; T *res; } alias = { prefix_::in_use };
            return alias.res;
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(prefix_data);
    };
    
}

#endif
