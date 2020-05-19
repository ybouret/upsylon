//! \file

#ifndef Y_SPADE_DISPATCHER_INCLUDED
#define Y_SPADE_DISPATCHER_INCLUDED 1

#include "y/spade/layouts.hpp"
#include "y/ios/conveyors.hpp"
#include "y/ios/ovstream.hpp"

namespace upsylon {
    
    namespace Spade
    {
        
        class Dispatcher
        {
        public:
            typedef ios::ovstream Block;
            
            explicit Dispatcher(const comms::topology where);
            virtual ~Dispatcher() throw();
            
            template <typename FIELD> inline
            void autoExchange(FIELD                                &F,
                              const Layouts<typename FIELD::coord> &L) const
            {
                assert(L.outer.isSameThan(F));
                size_t n = L.autoExchange.size();
                while(n-- > 0 )
                {
                    const AutoExchangeSwaps<typename FIELD::coord> &xch = L.autoExchange[n];
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
            
            
            const comms::topology topology;

            
            void asyncInitialize(Block &block) throw();
            
            template <typename FIELD>
            void asyncSave(Block       &block,
                           const FIELD &field,
                           const Ghost &ghost) const
            {
                static const ios::conveyor &io = IO.query< typename FIELD::mutable_type >(topology); assert(io.topo==topology);
                size_t                      n = ghost.items; assert(ghost.items==ghost.size());

                updateDelivery(io);
                while(n>0)
                {
                    typename FIELD::const_type &data = field( ghost[n] );
                    io.save(block, &data);
                    --n;
                }
            }

            

        protected:
            const comms::delivery delivery;
            ios::conveyors       &IO;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dispatcher);
            void updateDelivery(const ios::conveyor &) const throw();
        };
        
        
    }
    
}


#endif

