//! \file
#ifndef Y_CRYPTO_ISAAC_INCLUDED
#define Y_CRYPTO_ISAAC_INCLUDED 1

#include "y/randomized/isaac.hpp"
#include "y/crypto/sc/kstream.hpp"

namespace upsylon
{
	
	namespace crypto
	{
		//! ISAAC kstream
        class isaac_ks : public kstream
		{
		public:
            //! desctructor
			virtual ~isaac_ks() throw();

            //! setup
			explicit isaac_ks( const memory::ro_buffer & ) throw();

            //! reset
			virtual void      schedule( const memory::ro_buffer & ) throw();

            //! next byte
			virtual uint8_t   call() throw();
			
		private:
			Y_DISABLE_COPY_AND_ASSIGN(isaac_ks);
			randomized::isaac<8> isaac_;
		};
		
	}
	
}

#endif
