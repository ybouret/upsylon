#include "y/memory/storage.hpp"


namespace upsylon {

    namespace memory {


#define Y_MEM_STORAGE(W) case W : return #W

        const char *storage::text(const model which) throw()
        {
            switch(which)
            {
                    Y_MEM_STORAGE(unused);
                    Y_MEM_STORAGE(shared);
                    Y_MEM_STORAGE(pooled);
                    Y_MEM_STORAGE(global);
            }
            return "unknown";
        }

    }

}
