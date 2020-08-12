
#include "y/sibyl/provider.hpp"


namespace upsylon {

    Y_SINGLETON_IMPL(sibyl::provider);

    namespace sibyl {

        provider:: ~provider() throw()
        {
        }

        provider:: provider() throw() :
        singleton<provider>(),
        memory::tight::quarry(),
        memory::tight::quarry_allocator(access,*this)
        {
        }

    }

}


