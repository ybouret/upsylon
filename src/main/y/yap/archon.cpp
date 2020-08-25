

#include "y/yap/archon.hpp"
#include "y/object-parameters.hpp"


namespace upsylon {

    Y_SINGLETON_IMPL(yap::archon);

    namespace yap {

        archon:: ~archon() throw()
        {
        }

        archon:: archon() throw() :
        singleton<archon>(),
        memory::tight::quarry(),
        memory::tight::twain_allocator(access,
                                       *this,
                                       Y_CHUNK_SIZE,
                                       a_lower_size,
                                       a_limit_size)
        {
            
        }

    }

}

#include "y/yap/natural.hpp"


namespace upsylon {

    namespace yap {

        memory_allocator & natural::instance()
        {
            return archon::instance();
        }

        memory_allocator & natural::location() throw()
        {
            return archon::location();
        }


    }

}
