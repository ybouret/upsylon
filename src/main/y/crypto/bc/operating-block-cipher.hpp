//! \file
#ifndef Y_OP_CIPHER_INCLUDED
#define Y_OP_CIPHER_INCLUDED 1

#include "y/crypto/bc/block-cipher.hpp"

namespace upsylon {


	namespace crypto {


		//! the crypt operation is still virtual !
		class operating_block_cipher : public block_cipher
		{
		public:

			virtual ~operating_block_cipher() throw();

			virtual size_t size() const throw();
			
			//! residual crypting, 0<=len<size()
			void crypt_flush( void *output, const void *input, const size_t len ) throw();
			
			//! helper for any block
			void crypt_block( void *output, const void *input, const size_t len ) throw();
			
			//! blender interface
			//virtual void scramble( void *buffer, size_t length ) throw();
			virtual void schedule( const memory::ro_buffer &IV ) throw();
			
						
		protected:
			explicit operating_block_cipher(const char              *way,
                                            const pointer           &bcph,
                                            const pointer           &rcph,
                                            const memory::ro_buffer &IV );

			pointer           bc_;   //!< full block cipher
			pointer           rc_;   //!< residual bock cipher
			const size_t      size_; //!< common size
			digest            P_;    //!< Plain[i]
			digest            C_;    //!< Crypt[i]
			digest            Pp_;   //!< Plain[i-1]
			digest            Cp_;   //!< Crypt[i-1]
			digest            R_;    //!< residual helper



		private:
			Y_DISABLE_COPY_AND_ASSIGN(operating_block_cipher);
			

		};

	}

}

#endif
