
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
             - used as a cache of bytes=2^shift in interaction with
             the memory::global allocator calloc/free
             - all the blocks (a.k.a) ingots are used in a pool when
             they are store, so that they need to be big enough to hold sizeof(ingot)
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
                static  const size_t one      = 1;                       //!< alias
                static  const size_t min_exp2 = ilog2_of<ingot>::value;  //!< to hold a stone
                static  const size_t min_size = one << min_exp2;         //!< to hold a stone
                static  const size_t max_exp2 = (sizeof(size_t)<<3)-1;   //!< theoretical
                static  const size_t max_size = one << max_exp2;         //!< theoretical

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
                void * acquire();                      //!< get/create an ingot, !!may be dirty!!
                void   release(void *)        throw(); //!< store a previously allocated ingot
                size_t committed()      const throw(); //!< committed = count-chest.size
                size_t available()      const throw(); //!< chest.size
                void   optimize() throw();             //!< rearrange chest
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const size_t     block_exp2; //!< block_size = 1 << block_exp2
                const size_t     block_size; //!< block_size for each ingot
            private:
                chest_type       chest;      //!< available ingots
            public:
                const size_t     count;      //!< bookkeeping of allocated ingot

                //______________________________________________________________
                //
                // helpers
                //______________________________________________________________
                friend std::ostream & operator<<(std::ostream &, const vein &);  //!< display
                static unsigned       width;                                     //!< shared global width for ouptut

            private:
                Y_DISABLE_COPY_AND_ASSIGN(vein);
                void   free(ingot *) const throw(); //!< release uneeded ingot

            };

        }
    }
}
#endif
