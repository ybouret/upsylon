//! \file
#ifndef Y_TEAC_INCLUDED
#define Y_TEAC_INCLUDED 1

#include "y/crypto/bc/block-cipher.hpp"
#include "y/crypto/bc/tea.hpp"
#include <cstring>

namespace upsylon {
	
	namespace crypto {

        //! base class for TEA cipher
		class tea_cipher : public block_cipher
			{
			public:
				virtual ~tea_cipher() throw();       //!< cleanup
				virtual size_t size() const throw(); //!< block size
				
			protected:
				explicit tea_cipher( size_t block_size , const memory::ro_buffer &k ); //!< setup
				
				digest            k128_; //!< 128 bits key for tea_[encrypt|decrypt]
				const size_t      size_; //!< tea block size, >=8 and aligned on 4
				const size_t      nU32_; //!< tea dw_num
				digest            rU32_; //!< aligned
				
			private:
                Y_DISABLE_COPY_AND_ASSIGN(tea_cipher);
				
			};
		
		
		//! TEA cipher
		/**
		 BITS >= 64
		 */
		template <
		const size_t               BITS,
		const block_cipher::action MODE
		>
		class teac : public tea_cipher {
		public:
            //! destructor
			virtual ~teac() throw()
			{
			}

            //! setup
			explicit teac( const memory::ro_buffer &k ) :
			tea_cipher( BITS/8, k ),
			wksp_( (uint32_t *)rU32_.rw() ),
			kr32_( (const uint32_t *)k128_.ro())
			{
				
			}

            //! crypt blocks of 'size' bytes
			virtual void crypt( void *output, const void *input ) throw() 
			{
				memcpy( wksp_,  input, size_ );
				tea_exec( int2type<MODE>() );
				memcpy( output, wksp_, size_ );
			}
			
			
		private:
			uint32_t       *wksp_;
			const uint32_t *kr32_;
			
			teac( const teac & );
			teac&operator=( const teac & );
			
			inline
			void tea_exec(int2type<encrypting> ) throw() {
				tea_encrypt( wksp_, nU32_,  kr32_ );
			}
			
			inline
			void tea_exec( int2type<decrypting> ) throw() {
				tea_decrypt( wksp_, nU32_,  kr32_ );
			}
			
			
		};

        //! implementation of TEA cipher
		template <const size_t BITS>
		struct teac_of
		{
			typedef teac<BITS,block_cipher::encrypting> encrypter; //!< alias
			typedef teac<BITS,block_cipher::decrypting> decrypter; //!< alias
		};
		
		
	} //crypto
	
} //yocto




#endif
