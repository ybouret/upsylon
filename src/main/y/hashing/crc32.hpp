//! \file
#ifndef Y_HASHING_CRC32_INCLUDED
#define Y_HASHING_CRC32_INCLUDED 1

#include "y/hashing/function.hpp"

namespace upsylon
{
	namespace hashing
	{
        //! crc32
		class crc32 : public function
		{
		public:
            //! prototype
			Y_HASHING_FUNCTION_DECL(crc32,4,4);
			
		private:
			uint32_t    hash;
			Y_DISABLE_COPY_AND_ASSIGN(crc32);
		};
		
	}
}

#endif
