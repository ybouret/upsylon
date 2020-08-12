
//! \file

#ifndef Y_MEMORY_TIGHT_ZCACHE_INCLUDED
#define Y_MEMORY_TIGHT_ZCACHE_INCLUDED 1

#include "y/memory/tight/objects.hpp"

namespace upsylon {

    namespace memory {

        namespace tight {

            class xcache
            {
            public:
                explicit xcache(objects &objs, const size_t bs);
                virtual ~xcache() throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(xcache);
            };

        }

    }

}
