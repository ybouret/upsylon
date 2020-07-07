//! \file

#ifndef Y_MEMORY_MARKER_INCLUDED
#define Y_MEMORY_MARKER_INCLUDED 1

#include "y/parops.hpp"
#include <iosfwd>

namespace upsylon {

    namespace memory {

        //______________________________________________________________________
        //
        //! offset+length marker, with some algebra
        //______________________________________________________________________
        class marker
        {
        public:
            size_t offset; //!< the offset
            size_t length; //!< the lenght

            marker()                               throw(); //!< offset=0,length=0
            ~marker()                              throw(); //!< cleanup
            marker(const marker&)                  throw(); //!< copy
            marker & operator=(const marker &)     throw(); //!< assign
            marker(const size_t)                   throw(); //!< 0,length
            marker(const size_t,const size_t)      throw(); //!< offset,length
            marker(const marker &, const size_t t) throw(); //!< automatic offset from other marker,length
            size_t next_offset()              const throw(); //!< alignment for next marker

            //! MPI type splitting
            template <typename T>
            marker & split(const T size, const T rank) throw()
            {
                parops::split_any(length, offset, size, rank);
                return *this;
            }

            friend std::ostream & operator<<(std::ostream &, const marker &);

        };

    }

}

#endif

