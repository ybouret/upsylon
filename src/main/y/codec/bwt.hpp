//! \file
#ifndef Y_CODEC_BWT_INCLUDED
#define Y_CODEC_BWT_INCLUDED 1

#include "y/os/platform.hpp"


namespace upsylon
{
	
	namespace codec
	{
		
		struct bwt
		{
			static size_t encode( void *output, const void *input, const size_t size, size_t *indices) throw();
			static void   decode( void *output, const void *input, const size_t size, size_t *indices, const size_t primary_index) throw();

		};

	}
	
}


#endif
