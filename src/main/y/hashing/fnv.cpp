#include "y/hashing/fnv.hpp"

namespace upsylon
{
	namespace hashing
	{
		
		fnv:: fnv() throw() : function( __length, __window ), hash(0)
		{
		}
		
		fnv:: ~fnv() throw()
		{
			hash = 0;
		}
		
		const char fnv::CLID[] = "fnv";
		
		void fnv:: set() throw()
		{
			hash = 0;
		}
        
        static const uint32_t fnv_prime = 0x811C9DC5;

		void fnv:: run( const void *buffer, size_t buflen ) throw()
		{
			assert( !(buffer==NULL&&buflen>0) );
			const uint8_t *str = (const uint8_t *)buffer;
			
			for(;buflen>0; --buflen, ++str)
			{
				hash *= fnv_prime;
				hash ^= (*str);
			}
			
		}
		
        size_t fnv:: of(const char *text) throw()
        {
            assert(text);
            uint32_t       hash = 0;
            const uint8_t *addr = (const uint8_t *)text;
            while(*addr)
            {
                hash *= fnv_prime;
                hash ^= (*(addr++));
            }
            return hash;
        }
        
		void fnv:: get( void *output, size_t outlen ) throw()
		{
			fill(output, outlen, &hash, sizeof(hash) );
		}
		
		
	}
	
}

