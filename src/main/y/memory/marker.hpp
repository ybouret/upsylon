//! \file

#ifndef Y_MEMORY_MARKER_INCLUDED
#define Y_MEMORY_MARKER_INCLUDED 1

#include "y/parops.hpp"

namespace upsylon {

    namespace memory {

        class marker
        {
        public:
            size_t offset;
            size_t length;

            marker() throw();
            ~marker() throw();
            marker(const marker&) throw();
            marker & operator=(const marker &) throw();
            marker(const size_t,const size_t) throw();
            marker(const marker &, const size_t t) throw(); //!< automatic offset

            template <typename T>
            marker & split(const T size, const T rank) throw()
            {
                parops::split_any(length, offset, size, rank);
                return *this;
            }

        };

    }

}

#endif

