//! \file

#ifndef Y_MOMENTARY_LINK_INCLUDED
#define Y_MOMENTARY_LINK_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! make a temporary link...
    //
    //_________________________________________________________________________
    template <typename T>
    class momentary_link
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliasses

        //! handle = &source
        inline explicit momentary_link(type  &source,
                                       type **handle) throw():
        linked(handle)
        {
            assert(linked);
            assert(0==*linked);
            *linked = &source;
        }

        //! handle = NULL
        inline ~momentary_link() throw() { assert(linked); *linked = 0; }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(momentary_link);
        type **linked;
    };

}

#endif

