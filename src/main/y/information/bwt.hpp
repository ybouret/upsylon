//! \file
#ifndef Y_INFORMATION_BWT_INCLUDED
#define Y_INFORMATION_BWT_INCLUDED 1

#include "y/information/modulation.hpp"


namespace upsylon {
	
	namespace Information 	{
        
        //! Burrows Wheeler Transform
		struct bwt
		{
            //! forward transformation, returns the primary index
			static size_t encode(void        *output,
                                 const void  *input,
                                 const size_t size,
                                 size_t      *indices) throw();

            //! reverse transformation using the primary index
			static void   decode(void        *output,
                                 const void  *input,
                                 const size_t size,
                                 size_t      *indices,
                                 const size_t primary_index) throw();

            //! forward transformation with shaker
            static size_t encode(void        *output,
                                 const void  *input,
                                 const size_t size,
                                 size_t      *indices,
                                 Modulation  &proc) throw();

            //! reverse transformation using the primary index
            static void   decode(void        *output,
                                 void        *input,
                                 const size_t size,
                                 size_t      *indices,
                                 const size_t primary_index,
                                 Modulation  &proc) throw();
		};

	}
	
}


#endif
