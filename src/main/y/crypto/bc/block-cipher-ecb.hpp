//! \file
#ifndef Y_BLOCK_CIPHER_ECB_INCLUDED
#define Y_BLOCK_CIPHER_ECB_INCLUDED 1

#include "y/crypto/bc/operating-block-cipher.hpp"

namespace upsylon {
	
	namespace crypto {
		
		//! Electronic Code Book
		struct block_cipher_ecb {
			
			//! \f$ C_i = E_K( P_i ) \f$
			class encrypter : public operating_block_cipher {
			public:
				explicit encrypter(  const pointer &bc, const memory::ro_buffer &iv);
				virtual ~encrypter() throw();
				
				virtual void        crypt( void *output, const void *input ) throw();
				
			private:
                Y_DISABLE_COPY_AND_ASSIGN(encrypter);
			};
			
			//! \f$ P_i = D_K( C_i ) \f$
			class decrypter : public operating_block_cipher {
			public:
				//! rc must be the encrypter bc
				explicit decrypter(  const pointer &bc, const pointer &rc, const memory::ro_buffer &iv );
				virtual ~decrypter() throw();
				
				virtual void crypt( void *output, const void *input ) throw();
				
			private:
                Y_DISABLE_COPY_AND_ASSIGN(decrypter);
			};
			
			
		};
		
		
	}
	
}

#endif
