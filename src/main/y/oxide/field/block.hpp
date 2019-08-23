
//! \file

#ifndef Y_OXIDE_FIELD_BLOCK_INCLUDED
#define Y_OXIDE_FIELD_BLOCK_INCLUDED 1
#include "y/oxide/types.hpp"

namespace upsylon
{
    namespace Oxide
    {

        class FieldBlock
        {
        public:
            inline virtual ~FieldBlock() throw();

        protected:
            void *addr;
            
        private:
            size_t allocated;
            Y_DISABLE_COPY_AND_ASSIGN(FieldBlock);
        };

    }
}

#endif

