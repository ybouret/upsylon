

#include "y/yap/archon.hpp"


namespace upsylon {

    Y_SINGLETON_IMPL(yap::archon);

    namespace yap {

        archon:: ~archon() throw()
        {
        }

        archon:: archon() throw() :
        singleton<archon>(),
        memory::tight::quarry(),
        memory::tight::quarry_allocator(access,*this)
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