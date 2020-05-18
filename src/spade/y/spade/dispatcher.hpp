//! \file

#ifndef Y_SPADE_DISPATCHER_INCLUDED
#define Y_SPADE_DISPATCHER_INCLUDED 1

#include "y/spade/layouts.hpp"

namespace upsylon {
    
    namespace Spade
    {
        
        class Dispatcher
        {
        public:
            
            template <typename FIELD> inline
            void AutoExchange(FIELD                          &F,
                              Layouts<typename FIELD::coord> &L) const
            {
                assert(L.outer.isSameThan(F));
                const size_t n = L.autoExchange.size();
                
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dispatcher);
        };
        
        
    }
    
}


#endif

