//! \file
#ifndef Y_HASHING_FNV_INCLUDED
#define Y_HASHING_FNV_INCLUDED 1

#include "y/hashing/function.hpp"

namespace upsylon
{
	namespace hashing
	{

        //! FNV
		class fnv : public function
		{
		public:
            //! prototype
			Y_HASHING_FUNCTION_DECL(fnv,4,4);
			
		private:
			uint32_t hash;
			Y_DISABLE_COPY_AND_ASSIGN(fnv);
		};
		
	}
}

#endif
