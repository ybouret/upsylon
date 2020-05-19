
//! \file

#ifndef Y_SPADE_LAYOUT_SWAPS_INCLUDED
#define Y_SPADE_LAYOUT_SWAPS_INCLUDED 1

#include "y/spade/layout/ghosts.hpp"
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
            class Swaps : public Object, public Ghosts
            {
            public:
                //--------------------------------------------------------------
                //
                // types and definition
                //
                //--------------------------------------------------------------
                typedef Layout<COORD>  LayoutType; //!< alias
                typedef arc_ptr<Swaps> HandleType; //!< alia
                
                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                
                //! cleanup
                inline virtual ~Swaps() throw()
                {
                }
                
                //! setup from computed layouts and given peer
                explicit Swaps(const LayoutType &sendLayout,
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
                Y_DISABLE_COPY_AND_ASSIGN(Swaps);
            };
            
        }
        
        
        //----------------------------------------------------------------------
        //
        //! Swapping AutoExchange, for methods overloading
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class AutoExchangeSwaps
        {
        public:
            //! alias
            typedef typename Kernel::Swaps<COORD>::HandleType Swaps;
            
            //! cleanup
            inline ~AutoExchangeSwaps() throw() {}
            
            //! setup
            inline  AutoExchangeSwaps(const Swaps &fwd,
                                      const Swaps &rev) throw() :
            forward(fwd),
            reverse(rev)
            {
                assert(forward->peer==reverse->peer);
                assert(forward->innerGhost.items==reverse->outerGhost.items);
                assert(forward->outerGhost.items==reverse->innerGhost.items);
            }
            
            const Swaps forward; //!< forward swap
            const Swaps reverse; //!< reverse swap
            
            //! display
            friend inline std::ostream & operator<<( std::ostream &os, const AutoExchangeSwaps &s)
            {
                os << "fwd.snd: " << s.forward->innerRange <<  " | rcv: " << s.reverse->outerRange << std::endl;
                os << "rev.snd: " << s.reverse->innerRange <<  " | rcv: " << s.forward->innerRange;
                return os;
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AutoExchangeSwaps);
        };
        
        //----------------------------------------------------------------------
        //
        //! Swapping AsyncTwoWays, for methods overloading
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class AsyncTwoWaysSwaps
        {
        public:
            //! alias
            typedef typename Kernel::Swaps<COORD>::HandleType Swaps;
            
            //! cleanup
            inline ~AsyncTwoWaysSwaps() throw() {}
            
            //! setup
            inline  AsyncTwoWaysSwaps(const Swaps &fwd,
                                      const Swaps &rev) throw() :
            forward(fwd),
            reverse(rev)
            {
                assert(forward->innerGhost.items==reverse->outerGhost.items);
                assert(forward->outerGhost.items==reverse->innerGhost.items);
            }
            
            const Swaps forward; //!< forward swap
            const Swaps reverse; //!< reverse swap
            
            //! display
            friend inline std::ostream & operator<<( std::ostream &os, const AsyncTwoWaysSwaps &s)
            {
                os << "fwd.snd: " << s.forward->innerRange <<  " | rcv: " << s.reverse->outerRange << std::endl;
                os << "rev.snd: " << s.reverse->innerRange <<  " | rcv: " << s.forward->innerRange;
                return os;
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AsyncTwoWaysSwaps);
        };
        
        //----------------------------------------------------------------------
        //
        //! Swapping AsyncForward, for methods overloading
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class AsyncForwardSwaps
        {
        public:
            //! alias
            typedef typename Kernel::Swaps<COORD>::HandleType Swaps;
            
            //! cleanup
            inline ~AsyncForwardSwaps() throw() {}
            
            //! setup
            inline  AsyncForwardSwaps(const Swaps &fwd) throw() :
            forward(fwd)
            {
            }

            const Swaps forward; //!< forward only
            
            //! display
            friend inline std::ostream & operator<<( std::ostream &os, const AsyncForwardSwaps &s)
            {
                os << "fwd.snd: " << s.forward->innerRange <<  " | rcv: " << s.forward->outerRange << std::endl;
                return os;
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AsyncForwardSwaps);
            
        };
        
        //----------------------------------------------------------------------
        //
        //! Swapping AsyncReverse, for methods overloading
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class AsyncReverseSwaps
        {
        public:
            //! alias
            typedef typename Kernel::Swaps<COORD>::HandleType Swaps;
            
            //! cleanup
            inline ~AsyncReverseSwaps() throw() {}
            
            //! setup
            inline  AsyncReverseSwaps(const Swaps &rev) throw() :
            reverse(rev)
            {
            }
            
            const Swaps reverse; //!< reverse only
            
            //! display
            friend inline std::ostream & operator<<( std::ostream &os, const AsyncReverseSwaps &s)
            {
                os << "rev.snd: " << s.reverse->innerRange <<  " | rcv: " << s.reverse->outerRange;
                return os;
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AsyncReverseSwaps);
        };
        
        
    }
    
}

#endif

