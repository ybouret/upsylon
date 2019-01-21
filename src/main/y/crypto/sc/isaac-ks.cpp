#include "y/crypto/sc/isaac-ks.hpp"

namespace upsylon
{
    
    namespace crypto
    {
        
        isaac_ks:: ~isaac_ks() throw()
        {
        }
        
        isaac_ks:: isaac_ks( const memory::ro_buffer &key ) throw() :
        isaac_( key )
        {
        }
        
        void isaac_ks:: schedule( const memory::ro_buffer &key ) throw()
        {
            isaac_.reset( key );
        }
        
        uint8_t isaac_ks:: call() throw()
        {
            return uint8_t(isaac_.rand32());
        }
        
    }
    
}

