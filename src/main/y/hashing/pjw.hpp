//! \file
#ifndef Y_HASHING_PJW_INCLUDED
#define Y_HASHING_PJW_INCLUDED 1

#include "y/hashing/function.hpp"

namespace upsylon
{
	namespace hashing
	{

        //! PJW
		class pjw : public function
		{
		public:
            //! prototype
			Y_HASHING_FUNCTION_DECL(pjw,4,4);
			
		private:
			uint32_t hash;
			Y_DISABLE_COPY_AND_ASSIGN(pjw);
		};
		
	}
}

#endif
