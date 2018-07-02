
#include "y/memory/buffer.hpp"

namespace upsylon
{
    namespace memory
    {
        ro_buffer:: ~ro_buffer() throw()
        {
        }

        ro_buffer:: ro_buffer() throw()
        {
        }
    }
}

namespace upsylon
{
    namespace memory
    {
        rw_buffer:: ~rw_buffer() throw()
        {
        }

        rw_buffer:: rw_buffer() throw()
        {
        }

        void *rw_buffer:: rw() throw()
        {
            return (void*)ro();
        }
    }
}
