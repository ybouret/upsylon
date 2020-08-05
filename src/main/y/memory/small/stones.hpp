
//! \file

#ifndef Y_MEMORY_SMALL_STONES_INCLUDED
#define Y_MEMORY_SMALL_STONES_INCLUDED 1

#include "y/memory/small/stone.hpp"
#include "y/code/ilog2.hpp"
#include "y/core/list.hpp"

#include <iosfwd>

namespace upsylon {

    namespace memory {

        namespace small {


            class stones
            {
            public:
                typedef core::list_of<stone> slist_type;
                static const size_t one       = 1;
                static const size_t min_shift = ilog2_of<stone>::value;
                static const size_t min_bytes = one << min_shift;
                static const size_t max_shift = (sizeof(size_t)<<3)-1;
                static const size_t max_bytes = one << max_shift;
                static unsigned     width; //!< shared global width for ouptu

                const size_t shift; //!< bytes = 1 << shift
                const size_t bytes; //!< bytes for each stone
                slist_type   slist; //!< available stones
                const size_t count; //!< bookkeeping of allocated stones

                stones(const size_t usr_shift) throw();
                ~stones() throw();

                //! get/create a stone, memory may be garbage
                void *query();

                //! store a previous stone, keep memory ordered
                void  store(void *) throw();

                //! release unneeded stone
                void  release(stone *) const throw();

                //! committed = count-slits.size
                size_t committed() const throw();

                //! display
                friend std::ostream & operator<<(std::ostream &, const stones &);
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(stones);
            };

        }
    }
}
#endif
