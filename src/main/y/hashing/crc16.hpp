#ifndef Y_HASHING_CRC16_INCLUDED
#define Y_HASHING_CRC16_INCLUDED 1

#include "y/hashing/function.hpp"

namespace upsylon
{
	namespace hashing
	{
		class crc16 : public function
		{
		public:
			Y_HASHING_FUNCTION_DECL(crc16,2,2);
			
		private:
			uint16_t    hash;
			Y_DISABLE_COPY_AND_ASSIGN(crc16);
		};
		
	}
}

#endif
