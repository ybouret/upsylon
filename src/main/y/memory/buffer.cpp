#include "y/memory/buffer.hpp"
#include "y/code/utils.hpp"

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

        const char * ro_buffer:: hexa_at(const size_t i, const CharCase t) const throw()
        {
            switch(t)
            {
                case Lowercase: return hexadecimal::lowercase[ byte_at(i) ];
                case Uppercase: return hexadecimal::uppercase[ byte_at(i) ];
            }
            return "??";
        }
        
        bool ro_buffer:: is_zeroed() const throw()
        {
            const uint8_t *ptr = static_cast<const uint8_t *>( ro() );
            const size_t   len = length();
            for(size_t i=0;i<len;++i)
            {
                if( 0 != ptr[i] )
                {
                    return false;
                }
            }
            return true;
        }

    }
}

#include "y/type/cswap.hpp"

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
