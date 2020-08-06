//! \file

#ifndef Y_MEMORY_SMALL_MT_CACHE_INCLUDED
#define Y_MEMORY_SMALL_MT_CACHE_INCLUDED 1

#include "y/lockable.hpp"


namespace upsylon {

    namespace memory {

        namespace small {

            class mt_cache
            {
            public:
                virtual ~mt_cache() throw();


            private:
                virtual void *on_query();
                virtual void  on_store(void *) throw();
                Y_DISABLE_COPY_AND_ASSIGN(mt_cache);
            };

        }
    }
}

#endif


