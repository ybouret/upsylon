
//! \file

#ifndef Y_SPADE_SWAPS_INCLUDED
#define Y_SPADE_SWAPS_INCLUDED 1

#include "y/spade/ghosts.hpp"
#include "y/spade/layout.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {
    
    namespace Spade {
        
        namespace Kernel {
            
            //------------------------------------------------------------------
            //
            //! ghosts zones with their layouts
            //
            //------------------------------------------------------------------
            template <typename COORD>
            class Swap : public Object, public Ghosts
            {
            public:
                //--------------------------------------------------------------
                //
                // types and definition
                //
                //--------------------------------------------------------------
                typedef Layout<COORD> LayoutType; //!< alias
                typedef arc_ptr<Swap> HandleType; //!< alia
                
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                
                //! cleanup
                inline virtual ~Swap() throw()
                {
                }
                
                //! setup from computed layouts and given peer
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
                
                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const LayoutType innerRange; //!< for innerGhost/send
                const LayoutType outerRange; //!< for outerGhost/recv
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Swap);
            };
            
        }
        
        
        //----------------------------------------------------------------------
        //
        //! Swapping AutoExchange, for methods overloading
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class AutoExchangeSwap
        {
        public:
            //! alias
            typedef typename Kernel::Swap<COORD>::HandleType Swap;
            
            //! cleanup
            inline ~AutoExchangeSwap() throw() {}
            
            //! setup
            inline  AutoExchangeSwap(const Swap &fwd,
                                     const Swap &rev) throw() :
            forward(fwd),
            reverse(rev)
            {
                assert(fwd->peer==rev->peer);
            }
            
            const Swap forward; //!< forward swap
            const Swap reverse; //!< reverse swap
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AutoExchangeSwap);
        };
        
        //----------------------------------------------------------------------
        //
        //! Swapping AsyncTwoWays, for methods overloading
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class AsyncTwoWaysSwap
        {
        public:
            //! alias
            typedef typename Kernel::Swap<COORD>::HandleType Swap;
            
            //! cleanup
            inline ~AsyncTwoWaysSwap() throw() {}
            
            //! setup
            inline  AsyncTwoWaysSwap(const Swap &fwd,
                                     const Swap &rev) throw() :
            forward(fwd),
            reverse(rev)
            {
                assert(fwd->peer!=rev->peer);
            }
            
            const Swap forward; //!< forward swap
            const Swap reverse; //!< reverse swap
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AsyncTwoWaysSwap);
        };
        
        //----------------------------------------------------------------------
        //
        //! Swapping AsyncForward, for methods overloading
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class AsyncForwardSwap
        {
        public:
            //! alias
            typedef typename Kernel::Swap<COORD>::HandleType Swap;
            
            //! cleanup
            inline ~AsyncForwardSwap() throw() {}
            
            //! setup
            inline  AsyncForwardSwap(const Swap &fwd) throw() :
            forward(fwd)
            {
            }
             
            const Swap forward; //!< forward only
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AsyncForwardSwap);
            
        };
        
        //----------------------------------------------------------------------
        //
        //! Swapping AsyncReverse, for methods overloading
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class AsyncReverseSwap
        {
        public:
            //! alias
            typedef typename Kernel::Swap<COORD>::HandleType Swap;
            
            //! cleanup
            inline ~AsyncReverseSwap() throw() {}
            
            //! setup
            inline  AsyncReverseSwap(const Swap &rev) throw() :
            reverse(rev)
            {
            }
            
            const Swap reverse; //!< reverse only
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AsyncReverseSwap);
        };
        
        
    }
    
}

#endif

