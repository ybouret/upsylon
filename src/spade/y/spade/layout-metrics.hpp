//! \file

#ifndef Y_SPADE_LAYOUT_METRICS_INCLUDED
#define Y_SPADE_LAYOUT_METRICS_INCLUDED 1

#include "y/spade/types.hpp"

namespace upsylon {
 
    namespace Spade
    {
        
        class LayoutMetrics
        {
        public:
            virtual ~LayoutMetrics() throw();
            
            const unsigned dimensions;  //!< dimensions
            const size_t   items;       //!< linear items for this layout
            
        
        protected:
            explicit LayoutMetrics(const unsigned d) throw();
            
        private:
            Y_DISABLE_ASSIGN(LayoutMetrics);
        };
        
    }
    
}

#endif

