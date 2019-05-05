//! \file
#ifndef Y_MEMORY_VEIN_INCLUDED
#define Y_MEMORY_VEIN_INCLUDED 1

#include "y/memory/allocator.hpp"
#include "y/code/ilog2.hpp"

namespace upsylon
{
    namespace memory
    {
        //! an allocator of power of two aligned blocks
        class vein : public allocator
        {
        public:
            static const size_t       min_bits   = ilog2<sizeof(size_t)>::value; //!< minimal bits per block
            static const size_t       max_bits   = 12;                           //!< maximal bits per block
            static const size_t       count      = (1+max_bits)-min_bits;        //!< number of different blocks
            static const size_t       min_size   = 1 << min_bits;                //!< minimal bytes per block
            static const size_t       max_size   = 1 << max_bits;                //!< maximal bytes per block
            static const size_t       proto_size = 36 * sizeof(void*);          //!< sizeof nuggets<BLOCK_BITS>

            static size_t       bytes_for( const size_t length, size_t &ibit ); //!< computation or power of two size and corresponding most significant bit


            explicit vein() throw(); //!< construct nuggets, by metaprogramming
            virtual ~vein() throw(); //!< destruct nuggets,  by metaprogramming

            virtual void *acquire(size_t &n);                    //!< allocator interface : acquire
            virtual void  release(void * &p, size_t &n) throw(); //!< allocator interface : release

        private:
            Y_DISABLE_COPY_AND_ASSIGN(vein);
            char   workspace[count][proto_size];
        };
    }
}

#endif

