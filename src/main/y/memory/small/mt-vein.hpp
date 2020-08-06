

//! \file

#ifndef Y_MEMORY_SMALL_MT_VEIN_INCLUDED
#define Y_MEMORY_SMALL_MT_VEIN_INCLUDED 1

#include "y/memory/small/vein.hpp"
#include "y/memory/small/hoard.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            class mt_vein : public hoard
            {
            public:
                explicit mt_vein(lockable &l, vein &v) throw();
                virtual ~mt_vein() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(mt_vein);
                vein &proxy;
                virtual void *on_query();
                virtual void  on_store(void *) throw();
            };

        }
     }
}

#endif
