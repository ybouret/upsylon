//! \file
#ifndef Y_HASHING_RMD128_INCLUDED
#define Y_HASHING_RMD128_INCLUDED 1


#include "y/hashing/function.hpp"
#include "y/hashing/rmd.hpp"

namespace upsylon
{
    namespace hashing
    {
        //! RipeMD-128
        class rmd128 : public function
		{
		public:
			Y_HASHING_FUNCTION_DECL(rmd128,16,64); //!< specs
			static const size_t RMDsize=128;       //!< 128 bits
            
            
		private:
			Y_DISABLE_COPY_AND_ASSIGN(rmd128);
            rmd      RMD;
            uint32_t MDbuf[RMDsize/32];
		};
    }
}

#endif
