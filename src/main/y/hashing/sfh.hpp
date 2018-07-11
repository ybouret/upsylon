//! \file
#ifndef Y_HASHING_SFH_INCLUDED
#define Y_HASHING_SFH_INCLUDED 1

#include "y/hashing/function.hpp"

namespace upsylon
{
	namespace hashing
	{

        //! Super Fast Hash
		class sfh : public function
		{
		public:
            //! prototype
			Y_HASHING_FUNCTION_DECL(sfh,4,4);
			
		private:
			uint32_t hash;
			Y_DISABLE_COPY_AND_ASSIGN(sfh);
		};
		
	}
}

#endif
