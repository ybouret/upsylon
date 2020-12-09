
//! \file
#ifndef Y_MEMORY_CBLOCKS_INCLUDED
#define Y_MEMORY_CBLOCKS_INCLUDED 1

#include "y/memory/cblock.hpp"


namespace upsylon
{

    namespace memory
    {

        //! cache of blocks
        class cblocks
        {
        public:
            explicit cblocks();
            virtual ~cblocks() throw();

            struct block_type
            {
                block_type *next;
                block_type *prev;
                void       *addr;
                size_t      exp2;
                size_t      size;
            };

        private:
            
        };
        
    }


}

#endif
