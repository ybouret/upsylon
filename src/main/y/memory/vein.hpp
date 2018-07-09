//! \file
#ifndef Y_MEMORY_VEIN_INCLUDED
#define Y_MEMORY_VEIN_INCLUDED 1

#include "y/memory/nuggets.hpp"
#include "y/code/ilog2.hpp"

namespace upsylon
{
    namespace memory
    {
        class vein : public allocator
        {
        public:
            static const size_t       min_bits = ilog2<sizeof(size_t)>::value;
            static const size_t       max_bits = 11;//sizeof(uint32_t)*8-2;
            static const size_t       count    = (1+max_bits)-min_bits;
            static const size_t       min_size = 1 << min_bits;
            static const size_t       max_size = 1 << max_bits;
            typedef nuggets<min_bits> proto;
            static const size_t       psize = sizeof(proto);

            static size_t       bytes_for( const size_t length, size_t &bits );


            explicit vein() throw();
            virtual ~vein() throw();

            void *acquire(size_t &n);
            void  release(void * &p, size_t &n) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(vein);
            proto *entry;
            char   workspace[count][psize];
        };
    }
}

#endif

