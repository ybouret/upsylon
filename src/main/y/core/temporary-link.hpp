//! \file

#ifndef Y_CORE_TEMPORARY_LINK_INCLUDED
#define Y_CORE_TEMPORARY_LINK_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon {

    namespace core
    {
        template <typename T>
        class temporary_link
        {
        public:
            Y_DECL_ARGS(T,type);

            inline explicit temporary_link(type  &source,
                                           type **handle) :
            linked(handle)
            {
                assert(linked);
                assert(0==*linked);
                *linked = &source;
            }

            inline ~temporary_link() throw()
            {
                *linked = 0;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(temporary_link);
            type **linked;
        };
    }
}

#endif

