//! \file
#ifndef Y_ARC4_INCLUDED
#define Y_ARC4_INCLUDED 1

#include "y/crypto/sc/kstream.hpp"

namespace upsylon
{
	
	namespace crypto
	{

        //! Alledged RC4
        class arc4 : public kstream
		{
		public:
            //! setup
			explicit arc4( const memory::ro_buffer &key ) throw();

            //! cleanup
			virtual ~arc4() throw();

            //! reset with a given key
			virtual void     schedule( const memory::ro_buffer &key ) throw();

            //! next byte
			virtual uint8_t  call() throw();
			
		private:
			unsigned x;
			unsigned y;
			uint8_t  S[256];
			void    init( const uint8_t *key, size_t len ) throw();
		};
		
		
	}
}

#endif
