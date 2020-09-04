//! \file

#ifndef Y_CORE_TEMPORARY_LINK_INCLUDED
#define Y_CORE_TEMPORARY_LINK_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon {

    namespace core
    {
        //______________________________________________________________________
        //
        //! make a temporary link...
        //______________________________________________________________________
        template <typename T>
        class temporary_link
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliasses

            //! handle = &source
            inline explicit temporary_link(type  &source,
                                           type **handle) :
            linked(handle)
            {
                assert(linked);
                assert(0==*linked);
                *linked = &source;
            }

            //! handle = NULL
            inline ~temporary_link() throw() { assert(linked); *linked = 0; }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(temporary_link);
            type **linked;
        };
    }
}

#endif

