#include "y/concurrent/executable.hpp"

namespace upsylon
{

    namespace concurrent
    {
        executable:: ~executable() throw()
        {
        }

        executable:: executable() :
        access(),
        topo( new topology() )
        {
        }


        executable:: launcher:: ~launcher() throw()
        {
        }

        executable:: launcher:: launcher(executable  &user_host,
                                         const size_t user_size,
                                         const size_t user_rank) :
        thread(stub,this,user_size,user_rank),
        host(user_host)
        {
        }

        void executable:: launcher:: stub(void *addr) throw()
        {
            assert(addr);
            launcher &self = *static_cast<launcher *>(addr);
            self.host.call(self);
        }

    }

}
