//! \file

#ifndef Y_GFX_FILTERS_SOBEL_INCLUDED
#define Y_GFX_FILTERS_SOBLE_INCLUDED 1

#include "y/gfx/filters.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        
        class Sobel3 : public filters
        {
        public:
            static const char ID[];
            explicit Sobel3();
            virtual ~Sobel3() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Sobel3);
        };
        
    }
    
}

#endif
