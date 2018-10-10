//! \file
#ifndef Y_HASHING_RMD160_INCLUDED
#define Y_HASHING_RMD160_INCLUDED 1


#include "y/hashing/function.hpp"
#include "y/hashing/rmd.hpp"

namespace upsylon
{
    namespace hashing
    {
        
        class rmd160 : public function
		{
		public:
			Y_HASHING_FUNCTION_DECL(rmd160,20,64);
			static const size_t RMDsize=160;
            
            
		private:
			Y_DISABLE_COPY_AND_ASSIGN(rmd160);
            rmd      RMD;
            uint32_t MDbuf[RMDsize/32];
		};
    }
}

#endif
