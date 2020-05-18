
//! \file

#ifndef Y_SPADE_SWAPS_INCLUDED
#define Y_SPADE_SWAPS_INCLUDED 1

#include "y/spade/ghosts.hpp"
#include "y/spade/layout.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {
    
    namespace Spade {
        
        namespace Kernel {
            
            //! ghosts zones with their layouts
            template <typename COORD>
            class Swap : public Object, public Ghosts
            {
            public:
                typedef Layout<COORD> LayoutType;
                typedef arc_ptr<Swap> HandleType;
                
                inline virtual ~Swap() throw()
                {
                }
                
                explicit Swap(const LayoutType &sendLayout,
                              const LayoutType &recvLayout,
                              const LayoutType &fullLayout,
                              const size_t      peerRank) :
                Object(),
                Ghosts(sendLayout,recvLayout,fullLayout,peerRank),
                innerRange(sendLayout),
                outerRange(recvLayout)
                {
                    assert(innerRange.items==innerGhost.items);
                    assert(outerRange.items==outerGhost.items);

                }
                
                const LayoutType innerRange;
                const LayoutType outerRange;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Swap);
            };
            
        }
        
        
        
        template <typename COORD>
        class AutoExchangeSwap
        {
        public:
            typedef typename Kernel::Swap<COORD>::HandleType Swap;
            
            inline ~AutoExchangeSwap() throw() {}
            inline  AutoExchangeSwap(const Swap &fwd,
                                     const Swap &rev) throw() :
            forward(fwd),
            reverse(rev)
            {
                assert(fwd->peer==rev->peer);
            }
            
            const Swap forward;
            const Swap reverse;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AutoExchangeSwap);
        };
        
        template <typename COORD>
        class AsyncTwoWaysSwap
        {
        public:
            typedef typename Kernel::Swap<COORD>::HandleType Swap;
            
            inline ~AsyncTwoWaysSwap() throw() {}
            inline  AsyncTwoWaysSwap(const Swap &fwd,
                                     const Swap &rev) throw() :
            forward(fwd),
            reverse(rev)
            {
                assert(fwd->peer!=rev->peer);
            }
            
            const Swap forward;
            const Swap reverse;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AsyncTwoWaysSwap);
        };
        
        template <typename COORD>
        class AsyncForwardSwap
        {
        public:
            typedef typename Kernel::Swap<COORD>::HandleType Swap;
            
            inline ~AsyncForwardSwap() throw() {}
            inline  AsyncForwardSwap(const Swap &fwd) throw() :
            forward(fwd)
            {
            }
            
            const Swap forward;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AsyncForwardSwap);
            
        };
        
        template <typename COORD>
        class AsyncReverseSwap
        {
        public:
            typedef typename Kernel::Swap<COORD>::HandleType Swap;
            
            inline ~AsyncReverseSwap() throw() {}
            inline  AsyncReverseSwap(const Swap &rev) throw() :
            reverse(rev)
            {
            }
            
            const Swap reverse;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AsyncReverseSwap);
        };
        
        
    }
    
}

#endif

