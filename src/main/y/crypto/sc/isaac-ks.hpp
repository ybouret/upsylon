//! \file
#ifndef Y_CRYPTO_ISAAC_INCLUDED
#define Y_CRYPTO_ISAAC_INCLUDED 1

#include "y/randomized/isaac.hpp"
#include "y/crypto/sc/kstream.hpp"

namespace upsylon
{
	
	namespace crypto
	{
		
        class isaac_ks : public kstream
		{
		public:
			virtual ~isaac_ks() throw();
			explicit isaac_ks( const memory::ro_buffer & ) throw();
			
			virtual void      schedule( const memory::ro_buffer & ) throw();
			virtual uint8_t   call() throw();
			
		private:
			Y_DISABLE_COPY_AND_ASSIGN(isaac_ks);
			randomized::isaac<8> isaac_;
		};
		
	}
	
}

#endif
