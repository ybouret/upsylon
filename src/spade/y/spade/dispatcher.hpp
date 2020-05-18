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
            explicit Dispatcher();
            virtual ~Dispatcher() throw();
            
            template <typename FIELD> inline
            void autoExchange(FIELD                                &F,
                              const Layouts<typename FIELD::coord> &L) const
            {
                assert(L.outer.isSameThan(F));
                size_t n = L.autoExchange.size();
                while(n-- > 0 )
                {
                    const AutoExchangeSwap<typename FIELD::coord> &xch = L.autoExchange[n];
                    assert(F.contains(xch.forward->innerRange));
                    assert(F.contains(xch.forward->outerRange));
                    assert(F.contains(xch.reverse->outerRange));
                    assert(F.contains(xch.reverse->innerRange));
                    const accessible<size_t> & innerFwd = xch.forward->innerGhost;
                    const accessible<size_t> & outerFwd = xch.forward->outerGhost;
                    const accessible<size_t> & innerRev = xch.reverse->innerGhost;
                    const accessible<size_t> & outerRev = xch.reverse->outerGhost;
                    for(size_t i=innerFwd.size();i>0;--i)
                    {
                        F(outerFwd[i]) = F(innerRev[i]);
                        F(outerRev[i]) = F(innerFwd[i]);
                    }
                }
            }
            
            
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dispatcher);
        };
        
        
    }
    
}


#endif

