#ifndef Y_HASHING_ADLER32_INCLUDED
#define Y_HASHING_ADLER32_INCLUDED 1

#include "y/hashing/function.hpp"

namespace upsylon
{
	namespace hashing
	{
		
		class adler32 : public function
		{
		public:
			Y_HASHING_FUNCTION_DECL(adler32,4,4);
			
		private:
			uint32_t a;
			Y_DISABLE_COPY_AND_ASSIGN(adler32);
		};
		
	}
}

#endif
