
//! \file

#ifndef Y_MEMORY_TIGHT_VEIN_INCLUDED
#define Y_MEMORY_TIGHT_VEIN_INCLUDED 1

#include "y/memory/tight/ingot.hpp"
#include "y/code/ilog2.hpp"
#include "y/core/pool.hpp"
#include <iosfwd>

namespace upsylon {

    namespace memory {

        namespace tight {

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
                typedef core::pool_of<ingot> chest_type;                 //!< alias
                static  const size_t one       = 1;                      //!< alias
                static  const size_t min_exp2 = ilog2_of<ingot>::value;  //!< to hold a stone
                static  const size_t min_size = one << min_exp2;         //!< to hold a stone
                static  const size_t max_exp2 = (sizeof(size_t)<<3)-1;   //!< theoretical
                static  const size_t max_size = one << max_exp2;         //!< theoretical
                static  unsigned     width;                              //!< shared global width for ouptut



                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                vein(const size_t the_exp2) throw(); //!< create vein for ingots of size=1<<the_exp2
                ~vein() throw();                     //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                void * acquire();                      //!< get/create a stone, may be dirty
                void   release(void *)        throw(); //!< store a previous stone
                size_t committed()      const throw(); //!< committed = count-cache.size
                
                //! display
                friend std::ostream & operator<<(std::ostream &, const vein &);
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const size_t block_exp2; //!< block_size = 1 << block_exp2
                const size_t block_size; //!< block_size for each ingot
                chest_type   chest;      //!< available ingots
                const size_t count;      //!< bookkeeping of allocated ingot

            private:
                Y_DISABLE_COPY_AND_ASSIGN(vein);
                void   free(ingot *) const throw(); //!< release uneeded stone

            };

        }
    }
}
#endif
