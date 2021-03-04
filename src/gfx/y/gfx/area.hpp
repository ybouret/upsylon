//! \file

#ifndef Y_GFX_AREA_INCLUDED
#define Y_GFX_AREA_INCLUDED 1

#include "y/gfx/types.hpp"

namespace upsylon
{
    namespace graphic
    {
        
        class area
        {
        public:
            const unit_t w;
            const unit_t h;
            const size_t items;
            const coord  lower;
            const coord  upper;
            
            explicit area(const unit_t W, const unit_t H, const unit_t X=0, const unit_t Y=0);
            area(const area &) throw();
            virtual ~area() throw();
            
            friend std::ostream & operator<<(std::ostream &, const area &);
            
        private:
            Y_DISABLE_ASSIGN(area);
        };
        
        
    }
}


#endif

