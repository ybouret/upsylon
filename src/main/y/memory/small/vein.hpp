
//! \file

#ifndef Y_MEMORY_SMALL_VEIN_INCLUDED
#define Y_MEMORY_SMALL_VEIN_INCLUDED 1

#include "y/memory/small/ingot.hpp"
#include "y/code/ilog2.hpp"
#include "y/core/pool.hpp"
#include <iosfwd>

namespace upsylon {

    namespace memory {

        namespace small {

            //__________________________________________________________________
            //
            //
            //! (de)allocate ingots of same 2^shift bytes
            /**
             used as a cache of bytes=2^shift in interaction with
             the memory::global allocator calloc/free
             */
            //
            //__________________________________________________________________
            class vein
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef core::pool_of<ingot> cache_type;                 //!< alias
                static  const size_t one       = 1;                      //!< alias
                static  const size_t min_shift = ilog2_of<ingot>::value; //!< to hold a stone
                static  const size_t min_bytes = one << min_shift;       //!< to hold a stone
                static  const size_t max_shift = (sizeof(size_t)<<3)-1;  //!< theoretical
                static  const size_t max_bytes = one << max_shift;       //!< theoretical
                static  unsigned     width;                              //!< shared global width for ouptut



                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                vein(const size_t the_shift) throw(); //!< create vein for ingots of bytes=2^the_shift
                ~vein() throw();                      //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                void * query();                        //!< get/create a stone, set clean status
                void   store(void *)          throw(); //!< store a previous stone
                size_t committed()      const throw(); //!< committed = count-slist.size

                //! display
                friend std::ostream & operator<<(std::ostream &, const vein &);
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const size_t shift; //!< bytes = 1 << shift
                const size_t bytes; //!< bytes for each stone
                cache_type   cache; //!< available ingots
                const size_t count; //!< bookkeeping of allocated ingot

            private:
                Y_DISABLE_COPY_AND_ASSIGN(vein);
                void   release(ingot *) const throw(); //!< release uneeded stone

            };

        }
    }
}
#endif
