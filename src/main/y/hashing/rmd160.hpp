//! \file
#ifndef Y_HASHING_RMD160_INCLUDED
#define Y_HASHING_RMD160_INCLUDED 1


#include "y/hashing/function.hpp"
#include "y/hashing/rmd.hpp"

namespace upsylon
{
    namespace hashing
    {
        //! RipeMD 160
        class rmd160 : public function
		{
		public:
			Y_HASHING_FUNCTION_DECL(rmd160,20,64); //!< specs
			static const size_t RMDsize=160; //!< bits
            
            
		private:
			Y_DISABLE_COPY_AND_ASSIGN(rmd160);
            rmd      RMD;
            uint32_t MDbuf[RMDsize/32];
		};
    }
}

#endif
