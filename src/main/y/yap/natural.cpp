
#include "y/yap/natural.hpp"

namespace upsylon {

    namespace yap {

        natural:: ~natural() throw()
        {
            
        }

        size_t natural:: words_for(const size_t bytes) throw()
        {
            return Y_ROUND_LN2(word_exp2,bytes);
        }

        number::word_type * natural:: acquire(size_t &count, size_t &width, size_t &shift)
        {
            static memory_allocator &mgr = instance();
            return mgr.acquire_field<word_type>(count,width,shift);
        }

        void natural:: release(word_type *w, size_t &count, size_t &width, size_t &shift) throw()
        {
            static memory_allocator &mgr = instance();
            mgr.release_field(w,count,width,shift);
        }

    }

}


