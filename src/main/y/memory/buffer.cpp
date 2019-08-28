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

        uint8_t ro_buffer:: byte_at(size_t i) const throw()
        {
            return (i>=length()) ? 0 : *(static_cast<const uint8_t *>( ro() ) + i);
        }

    }
}

#include "y/type/bswap.hpp"

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

        void rw_buffer:: reverse() throw()
        {
            mreverse( rw(), length() );
        }
    }
}
