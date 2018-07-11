//! \file
#ifndef Y_HASHING_ELF_INCLUDED
#define Y_HASHING_ELF_INCLUDED 1

#include "y/hashing/function.hpp"

namespace upsylon
{
	namespace hashing
	{

        //! ELF
		class elf : public function
		{
		public:
            //! prototype
			Y_HASHING_FUNCTION_DECL(elf,4,4);
			
		private:
			uint32_t hash;
			Y_DISABLE_COPY_AND_ASSIGN(elf);
		};
		
	}
}

#endif
