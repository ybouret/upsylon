
//! \file

#ifndef Y_SPADE_LAYOUT_FRAGMENT_INCLUDED
#define Y_SPADE_LAYOUT_FRAGMENT_INCLUDED 1

#include "y/spade/layout/dispatch.hpp"
#include "y/spade/layout/swaps.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {

    namespace Spade
    {
        
        namespace Kernel {

            //! common for Spade::Fragment
            struct Fragment
            {
                //! ghost info
                enum GhostLocation
                {
                    InnerGhost,
                    OuterGhost
                };
                
                //! raise an exception on invalid ghosts
                static void TooManyGhosts(const Coord1D       ng,
                                          const unsigned      level,
                                          const GhostLocation where);
            };
        }
        
        //----------------------------------------------------------------------
        //
        //! computing sub-layouts
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class Fragment : public Dispatch<COORD>::Node
        {
        public:
            //------------------------------------------------------------------
            //
            // Types and defintions
            //
            //------------------------------------------------------------------
            static const unsigned Dimensions =               Layout<COORD>::Dimensions; //!< alias
            static const unsigned Levels     =               Dispatch<COORD>::Levels;   //!< alias
            typedef               Layout<COORD>              LayoutType;                //!< alias
            typedef typename      LayoutType::coord          coord;                     //!< alias
            typedef typename      LayoutType::const_coord    const_coord;               //!< alias
            typedef typename      Dispatch<COORD>::Node      Node;                      //!< alias
            typedef typename      Dispatch<COORD>::Link      Link;                      //!< alias
            typedef typename      Dispatch<COORD>::Links     Links;                     //!< alias
            typedef typename      Kernel::Swaps<COORD>       Swaps;                     //!< alias
            typedef typename      Swaps::HandleType          HSwaps;                    //!< alias
            typedef               AutoExchangeSwaps<COORD>   AutoExchangeType;          //!< alias
            typedef               AsyncTwoWaysSwaps<COORD>   AsyncTwoWaysType;          //!< alias
            typedef               AsyncForwardSwaps<COORD>   AsyncForwardType;          //!< alias
            typedef               AsyncReverseSwaps<COORD>   AsyncReverseType;          //!< alias

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------

            //! cleanup
            inline virtual ~Fragment() throw() {}

            //! setup layouts
            /**
             \param fullLayout the original full layout
             \param localRanks the local ranks in the topology
             \param dispatch   the dispatch used to compute rank(s)
             \param boundaries Periodic boundary condtions
             \param numGhosts  number of ghosts
             */
            inline explicit Fragment(const Layout<COORD>   &fullLayout,
                                     const_coord            localRanks,
                                     const Dispatch<COORD> &dispatch,
                                     const_coord            boundaries,
                                     const Coord1D          numGhosts) :
            Node(localRanks,dispatch,boundaries),
            inner(fullLayout.split(dispatch.sizes,this->ranks)),
            outer(inner),
            _core(0),
            autoExchange(this->numAutoExchange),
            asyncTwoWays(this->numAsyncTwoWays),
            asyncForward(this->numAsyncForward),
            asyncReverse(this->numAsyncReverse),
            commScore(0)
            {
                const Coord1D  ng   = abs_of(numGhosts);
                
                //______________________________________________________________
                //
                //
                // use Node information to expand inner layout
                //
                //______________________________________________________________

                //--------------------------------------------------------------
                // initialize corners
                //--------------------------------------------------------------
                coord lower = inner.lower;
                coord upper = inner.upper;
                
                coord coreLower = lower;
                coord coreUpper = upper;
                
                // scan main levels (0..Dimensions-1)
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    const Links &l = this->links[dim];
                    switch(l.forward.connectMode)
                    {
                        case Connect::Zilch: break;
                        case Connect::Async:
                        case Connect::Local:
                            Coord::Of(upper,dim)     += ng;
                            Coord::Of(coreUpper,dim) -= ng;
                    }
                    
                    switch(l.reverse.connectMode)
                    {
                        case Connect::Zilch: break;
                        case Connect::Async:
                        case Connect::Local:
                            Coord::Of(lower,dim)     -= ng;
                            Coord::Of(coreLower,dim) += ng;
                    }
                }
                
                //--------------------------------------------------------------
                // recompute outer
                //--------------------------------------------------------------
                new ((void*)&outer) Layout<COORD>(lower,upper);
                
                //--------------------------------------------------------------
                // compute _core
                //--------------------------------------------------------------
                {
                    bool hasCore = true;
                    for(unsigned dim=0;dim<Dimensions;++dim)
                    {
                        if( Coord::Of(coreLower,dim) > Coord::Of(coreUpper,dim) )
                        {
                            hasCore = false;
                            break;
                        }
                    }
                    if(hasCore) aliasing::_(_core) = new LayoutType(coreLower,coreUpper);
                }
                
                //______________________________________________________________
                //
                //
                // compute associated swap zones in each level
                //
                //______________________________________________________________
                if(ng>0) createAllSwaps(ng);

            }


            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const LayoutType              inner;        //!< inner workspace
            const LayoutType              outer;        //!< contains exchange zones
            auto_ptr<const LayoutType>    _core;        //!< optional core zone
            const slots<AutoExchangeType> autoExchange; //!< autoExchange ghosts
            const slots<AsyncTwoWaysType> asyncTwoWays; //!< asyncTwoWays ghosts
            const slots<AsyncForwardType> asyncForward; //!< aysncForward ghosts
            const slots<AsyncReverseType> asyncReverse; //!< asyncReverse ghosts
            const size_t                  commScore;    //!< items to comms...
            
            //------------------------------------------------------------------
            //
            // helpers
            //
            //------------------------------------------------------------------

            //! check compatible layout
            inline bool matches( const LayoutType &L ) const throw()
            {
                return L.isSameThan(outer);
            }

            //! display info
            inline void displaySwaps() const
            {
                std::cerr << "\t<Swaps>"  << std::endl;
                if(this->numAutoExchange>0)
                {
                    std::cerr << "\t\t<AutoExchange #" << this->numAutoExchange << ">" << std::endl;
                    for(size_t i=0;i<this->numAutoExchange;++i)
                    {
                        std::cerr << autoExchange[i] << std::endl;
                    }
                    std::cerr << "\t\t<AutoExchange/>" << std::endl;
                }
                
                if(this->numAsyncTwoWays>0)
                {
                    std::cerr << "\t\t<AsyncTwoWays #" << this->numAsyncTwoWays << ">" << std::endl;
                    for(size_t i=0;i<this->numAsyncTwoWays;++i)
                    {
                        std::cerr << asyncTwoWays[i] << std::endl;
                    }
                    std::cerr << "\t\t<AsyncTwoWays/>" << std::endl;
                }
                
                if(this->numAsyncForward>0)
                {
                    std::cerr << "\t\t<AsyncForward #" << this->numAsyncForward << ">" << std::endl;
                    for(size_t i=0;i<this->numAsyncForward;++i)
                    {
                        std::cerr << asyncForward[i] << std::endl;
                    }
                    std::cerr << "\t\t<AsyncForward/>" << std::endl;
                }
                
                if(this->numAsyncReverse>0)
                {
                    std::cerr << "\t\t<AsyncReverse #" << this->numAsyncReverse << ">" << std::endl;
                    for(size_t i=0;i<this->numAsyncReverse;++i)
                    {
                        std::cerr << asyncReverse[i] << std::endl;
                    }
                    std::cerr << "\t\t<AsyncReverse/>" << std::endl;
                }
                
                
                std::cerr << "\t<Swaps/>" << std::endl;
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Fragment);
            inline void createAllSwaps(const Coord1D ng)
            {
                assert(ng>0);
                assert(0==commScore);
                for(unsigned level=0;level<Levels;++level)
                {
                    //----------------------------------------------------------
                    //
                    // check this level topology
                    //
                    //----------------------------------------------------------
                    
                    const Links &l       = this->links[level];
                    const Link  &forward = l.forward;
                    const Link  &reverse = l.reverse;
                    
                    assert(forward.probe== Dispatch<COORD>::Coordination::Probes[level]);
                    assert(reverse.probe==-Dispatch<COORD>::Coordination::Probes[level]);
                    assert(Connect::Authorized(forward.connectMode,reverse.connectMode));
                    
                    //----------------------------------------------------------
                    //
                    // create swaps
                    //
                    //----------------------------------------------------------
                    switch(l.connect)
                    {
                        case Connect:: FreeStanding:
                            break;
                            
                        case Connect:: AutoExchange: {
                            const HSwaps fwd = createSwaps(forward, ng, level);
                            const HSwaps rev = createSwaps(reverse, ng, level);
                            aliasing::_(autoExchange).template build<const HSwaps&, const HSwaps&>(fwd,rev);
                        } break;
                            
                        case Connect:: AsyncTwoWays:{
                            const HSwaps fwd = createSwaps(forward, ng, level);
                            const HSwaps rev = createSwaps(reverse, ng, level);
                            aliasing::_(asyncTwoWays).template build<const HSwaps&, const HSwaps&>(fwd,rev);
                            aliasing::_(commScore) += (fwd->innerGhost.items << 1);
                        } break;
                            
                        case Connect:: AsyncForward: {
                            const HSwaps fwd = createSwaps(forward, ng, level);
                            aliasing::_(asyncForward).template build<const HSwaps&>(fwd);
                            aliasing::_(commScore) += (fwd->innerGhost.items);
                        } break;
                            
                        case Connect:: AsyncReverse: {
                            const HSwaps rev = createSwaps(reverse, ng, level);
                            aliasing::_(asyncReverse).template build<const HSwaps&>(rev);
                            aliasing::_(commScore) += (rev->innerGhost.items << 1);
                        } break;
                            
                        default:
                            Connect::InvalidFlag(l.connect, Connect::Layouts);
                            break;
                    }
                }
                assert( autoExchange.size() == this->numAutoExchange );
                assert( asyncTwoWays.size() == this->numAsyncTwoWays );
                assert( asyncForward.size() == this->numAsyncForward );
                assert( asyncReverse.size() == this->numAsyncReverse );
            }
            
            inline
            Swaps *createSwaps(const Link    &link,
                               const Coord1D  ng,
                               const unsigned level) const
            {
                assert(ng>0);
                const Coord1D shift = ng-1;
                
                // initialize
                coord send_lower = inner.lower;
                coord send_upper = inner.upper;
                coord recv_lower = inner.lower;
                coord recv_upper = inner.upper;
                
                // probe each direction
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    const Coord1D pr = Coord::Of(link.probe,dim);
                    Coord1D      &sl = Coord::Of(send_lower,dim);
                    Coord1D      &su = Coord::Of(send_upper,dim);
                    Coord1D      &rl = Coord::Of(recv_lower,dim);
                    Coord1D      &ru = Coord::Of(recv_upper,dim);
                    switch(pr)
                    {
                        case 1: //! increasing along direction
                            sl = su - shift;
                            rl = su+1;
                            ru = rl+shift;
                            break;
                            
                        case -1: //! decreasing along direction
                            su = sl + shift;
                            ru = sl-1;
                            rl = ru-shift;
                            break;
                            
                        default:
                            assert(0==pr);
                            break;
                    }
                }
                
                const Layout<COORD> innerRange(send_lower,send_upper);
                const Layout<COORD> outerRange(recv_lower,recv_upper);
                
                if(!outer.contains(innerRange)) Kernel::Fragment::TooManyGhosts(ng,level,Kernel::Fragment::InnerGhost);
                if(!outer.contains(outerRange)) Kernel::Fragment::TooManyGhosts(ng,level,Kernel::Fragment::OuterGhost);

                
                return new Swaps(innerRange,outerRange,outer,link.rank);
            }
        };
    }

}

#endif

