
//! \file

#ifndef Y_ALCHEMY_EXTENT_INCLUDED
#define Y_ALCHEMY_EXTENT_INCLUDED 1

#include "y/os/platform.hpp"
#include <iostream>

namespace upsylon
{
    namespace alchemy
    {
        
        class extent
        {
        public:
            const size_t index;
            const double value;
            
            extent(const size_t i, const double v) throw();
            extent(const extent &) throw();
            ~extent() throw();
            
            friend std::ostream & operator<<(std::ostream &os, const extent &xi);
            
        private:
            Y_DISABLE_ASSIGN(extent);
        };
        
        class extents
        {
        public:
            const extent forward;
            const extent reverse;
            
            extents(const extent fwd, const extent rev) throw();
            extents(const extents &) throw();
            ~extents() throw();
            
            friend std::ostream & operator<<(std::ostream &os, const extents &);

            
        private:
            Y_DISABLE_ASSIGN(extents);
        };
        
    }
    
}


#endif
