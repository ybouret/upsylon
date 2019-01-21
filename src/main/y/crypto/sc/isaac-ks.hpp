//! \file
#ifndef Y_CRYPTO_ISAAC_INCLUDED
#define Y_CRYPTO_ISAAC_INCLUDED 1

#include "y/randomized/isaac.hpp"

namespace upsylon
{
	
	namespace crypto
	{
		
		class isaac_key_stream 
		{
		public:
			virtual ~isaac_key_stream() throw();
			explicit isaac_key_stream( const memory::ro_buffer & ) throw();
			
			virtual void      schedule( const memory::ro_buffer & ) throw();
			virtual uint8_t   call() throw();
			
		private:
			Y_DISABLE_COPY_AND_ASSIGN(isaac_key_stream);
			randomized::isaac<8> isaac_;
		};
		
	}
	
}

#endif
