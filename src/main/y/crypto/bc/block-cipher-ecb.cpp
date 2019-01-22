#include "block-cipher-ecb.hpp"
#include <cstring>

namespace upsylon {
    
    namespace crypto {
        
        
        
        block_cipher_ecb:: encrypter:: encrypter(  const pointer &bc, const memory::ro_buffer &iv) :
        operating_block_cipher( "ECB-ENC", bc, bc, iv )
        {
        }
        
        
        block_cipher_ecb:: encrypter:: ~encrypter() throw()
        {
        }
        
        void block_cipher_ecb:: encrypter:: crypt( void *output, const void *input ) throw() {
            
            memcpy( P_.rw(), input, size_ );
            bc_->crypt( C_.rw(), P_.ro() );
            memcpy( output, C_.ro(), size_ );
            
            C_._swp( Cp_ );
            P_._swp( Pp_ );
        }
        
        
        
        
        block_cipher_ecb:: decrypter:: decrypter(  const pointer &bc ,  const pointer &rc, const memory::ro_buffer &iv ) :
        operating_block_cipher( "ECB-DEC", bc, rc, iv )
        {
        }
        
        
        block_cipher_ecb:: decrypter:: ~decrypter() throw()
        {
        }
        
        void block_cipher_ecb:: decrypter:: crypt( void *output, const void *input ) throw() {
            memcpy( C_.rw(), input, size_ );
            bc_->crypt( P_.rw(), C_.ro());
            memcpy( output, P_.ro(), size_ );
            C_._swp( Cp_ );
            P_._swp( Pp_ );
        }
        
    }
    
}
