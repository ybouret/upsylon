//! \file
#ifndef Y_MEMORY_VEIN_INCLUDED
#define Y_MEMORY_VEIN_INCLUDED 1

#include "y/memory/nuggets.hpp"
#include "y/code/ilog2.hpp"

namespace upsylon
{
    namespace memory
    {
        //! an allocator of power of two aligned blocks
        class vein : public allocator
        {
        public:
            static const size_t       min_bits = ilog2<sizeof(size_t)>::value; //!< minimal bits per block
            static const size_t       max_bits = 11;                           //!< maximal bits per block
            static const size_t       count    = (1+max_bits)-min_bits;        //!< number of different blocks
            static const size_t       min_size = 1 << min_bits;                //!< minimal bytes per block
            static const size_t       max_size = 1 << max_bits;                //!< maximal bytes per block
            typedef nuggets<min_bits> proto;                                   //!< nuggets prototype for memory requirements
            static const size_t       psize = sizeof(proto);                   //!< prototype size for memory requirements

            static size_t       bytes_for( const size_t length, size_t &ibit ); //!< computation or power of two size and corresponding most significant bit


            explicit vein() throw(); //!< construct nuggets, by metaprogramming
            virtual ~vein() throw(); //!< destruct nuggets, by metaprogramming

            //! allocator interface : acquire
            virtual void *acquire(size_t &n);
            //! allocator interface : release
            void  release(void * &p, size_t &n) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(vein);
            proto *entry;
            char   workspace[count][psize];
        };
    }
}

#endif

