//! \file
#ifndef Y_HASHING_BJH32_INCLUDED
#define Y_HASHING_BJH32_INCLUDED 1

#include "y/hashing/function.hpp"

namespace upsylon
{
	namespace hashing
	{
		
		class bjh32 : public function
		{
		public:
			Y_HASHING_FUNCTION_DECL(bjh32,4,4);
			
		private:
			uint32_t hash;
			Y_DISABLE_COPY_AND_ASSIGN(bjh32);
		};
		
	}
}

#endif
