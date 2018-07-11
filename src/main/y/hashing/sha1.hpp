#ifndef Y_HASHING_SHA1_INCLUDED
#define Y_HASHING_SHA1_INCLUDED 1

#include "y/hashing/function.hpp"

namespace upsylon
{
	namespace hashing
	{
		
		namespace RFC3174
		{
			struct SHA1_CTX
			{
				uint32_t total[2];   /*!< number of bytes processed  */
				uint32_t state[5];   /*!< intermediate digest state  */
				uint8_t  buffer[64]; /*!< data block being processed */
			};
		}
		
		class sha1 : public function
		{
		public:
			Y_HASHING_FUNCTION_DECL(sha1,20,64);
			
		private:
			RFC3174::SHA1_CTX ctx;
			Y_DISABLE_COPY_AND_ASSIGN(sha1);
		};
		
	}
}

#endif
