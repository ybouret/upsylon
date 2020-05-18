
//! \file

#ifndef Y_SPADE_LAYOUTS_INCLUDED
#define Y_SPADE_LAYOUTS_INCLUDED 1

#include "y/spade/topology.hpp"
#include "y/spade/swaps.hpp"

namespace upsylon {

    namespace Spade
    {
        
        namespace Kernel {
         
            struct Layouts
            {
                enum GhostLocation
                {
                    InnerGhost,
                    OuterGhost
                };
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
        class Layouts : public Topology<COORD>::Node
        {
        public:
            //------------------------------------------------------------------
            //
            // Types and defintions
            //
            //------------------------------------------------------------------
            static const unsigned Dimensions =               Layout<COORD>::Dimensions; //!< alias
            static const unsigned Levels     =               Topology<COORD>::Levels;   //!< alias
            typedef typename      Layout<COORD>::coord       coord;                     //!< alias
            typedef typename      Layout<COORD>::const_coord const_coord;               //!< alias
            typedef typename      Topology<COORD>::Node      Node;                      //!< alias
            typedef typename      Topology<COORD>::Link      Link;                      //!< alias
            typedef typename      Topology<COORD>::Links     Links;                     //!< alias
            typedef typename      Topology<COORD>::Boolean   Boolean;                   //!< alias
            typedef typename      Kernel::Swap<COORD>        Swap;
            typedef typename      Swap::HandleType           SwapHandle;
            typedef               AutoExchangeSwap<COORD>    AutoExchangeType;
            typedef               AsyncTwoWaysSwap<COORD>    AsyncTwoWaysType;
            typedef               AsyncForwardSwap<COORD>    AsyncForwardType;
            typedef               AsyncReverseSwap<COORD>    AsyncReverseType;

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------

            //! cleanup
            inline virtual ~Layouts() throw() {}

            //! setup layouts
            /**
             \param fullLayout the original full layout
             \param localRanks the local ranks in the topology
             \param topology   the topology used to compute rank(s)
             \param boundaries Periodic boundary condtions
             \param numGhosts  number of ghosts
             */
            inline explicit Layouts(const Layout<COORD>   &fullLayout,
                                    const_coord            localRanks,
                                    const Topology<COORD> &topology,
                                    const Boolean         &boundaries,
                                    const Coord1D          numGhosts) :
            Node(localRanks,topology,boundaries),
            inner(fullLayout.split(topology.sizes,this->ranks)),
            outer(inner),
            autoExchange(this->numAutoExchange),
            asyncTwoWays(this->numAsyncTwoWays),
            asyncForward(this->numAsyncForward),
            asyncReverse(this->numAsyncReverse)
            {
                const Coord1D  ng   = abs_of(numGhosts);
                const Node    &self = *this;

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

                // scan main levels (0..Dimensions-1)
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    const Links &links = self[dim];
                    switch(links.forward.connectMode)
                    {
                        case Connect::Zilch: break;
                        case Connect::Async:
                        case Connect::Local:
                            Coord::Of(upper,dim) += ng;
                    }
                    
                    switch(links.reverse.connectMode)
                    {
                        case Connect::Zilch: break;
                        case Connect::Async:
                        case Connect::Local:
                            Coord::Of(lower,dim) -= ng;
                    }
                }
                
                //--------------------------------------------------------------
                // recompute outer
                //--------------------------------------------------------------
                new ((void*)&outer) Layout<COORD>(lower,upper);

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
            const Layout<COORD>     inner; //!< inner workspace
            const Layout<COORD>     outer; //!< contains exchange zones
            slots<AutoExchangeType> autoExchange;
            slots<AsyncTwoWaysType> asyncTwoWays;
            slots<AsyncForwardType> asyncForward;
            slots<AsyncReverseType> asyncReverse;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Layouts);
            inline void createAllSwaps(const Coord1D ng)
            {
                assert(ng>0);
                const Node &self = *this;
                for(unsigned level=0;level<Levels;++level)
                {
                    //----------------------------------------------------------
                    //
                    // check this level topology
                    //
                    //----------------------------------------------------------
                    
                    const Links &links   = self[level];
                    const Link  &forward = links.forward;
                    const Link  &reverse = links.reverse;
                    
                    assert(forward.probe== Topology<COORD>::Coordination::Probes[level]);
                    assert(reverse.probe==-Topology<COORD>::Coordination::Probes[level]);
                    assert(Connect::Authorized(forward.connectMode,reverse.connectMode));
                    
                    //----------------------------------------------------------
                    //
                    // create swaps
                    //
                    //----------------------------------------------------------
                    
                    switch(links.connect)
                    {
                        case Connect:: FreeStanding:
                            break;
                            
                        case Connect:: AutoExchange: {
                            const SwapHandle fwd = createSwap(forward, ng, level);
                            const SwapHandle rev = createSwap(reverse, ng, level);
                            aliasing::_(autoExchange).template build<const SwapHandle&, const SwapHandle&>(fwd,rev);
                        } break;
                            
                        case Connect:: AsyncTwoWays:{
                            const SwapHandle fwd = createSwap(forward, ng, level);
                            const SwapHandle rev = createSwap(reverse, ng, level);
                             aliasing::_(asyncTwoWays).template build<const SwapHandle&, const SwapHandle&>(fwd,rev);
                        } break;
                            
                        case Connect:: AsyncForward: {
                            const SwapHandle fwd = createSwap(forward, ng, level);
                             aliasing::_(asyncForward).template build<const SwapHandle&>(fwd);
                        } break;
                            
                        case Connect:: AsyncReverse: {
                            const SwapHandle rev = createSwap(reverse, ng, level);
                             aliasing::_(asyncReverse).template build<const SwapHandle&>(rev);
                        } break;
                            
                        default:
                            Connect::InvalidFlag(links.connect, Connect::Layouts);
                            break;
                    }
                }
                assert( autoExchange.size() == this->numAutoExchange );
                assert( asyncTwoWays.size() == this->numAsyncTwoWays );
                assert( asyncForward.size() == this->numAsyncForward );
                assert( asyncReverse.size() == this->numAsyncReverse );

            }
            
            inline
            Swap *createSwap(const Link    &link,
                             const Coord1D  ng,
                             const unsigned level) const
            {
                assert(ng>0);
                const Coord1D shift = ng-1;
               
                std::cerr << "..create swap " << link.modeText() <<  "->" << link.rank << std::endl;
                
                // initialize
                coord send_lower = inner.lower;
                coord send_upper = inner.upper;
                coord recv_lower = inner.lower;
                coord recv_upper = inner.upper;
                
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    const Coord1D pr = Coord::Of(link.probe,dim);
                    Coord1D      &sl = Coord::Of(send_lower,dim);
                    Coord1D      &su = Coord::Of(send_upper,dim);
                    Coord1D      &rl = Coord::Of(recv_lower,dim);
                    Coord1D      &ru = Coord::Of(recv_upper,dim);
                    switch(pr)
                    {
                        case 1:
                            sl = su - shift;
                            rl = su+1;
                            ru = rl+shift;
                            break;
                            
                        case -1:
                            break;
                            
                        default:
                            assert(0==pr);
                            break;
                    }
                }
                
                const Layout<COORD> innerRange(send_lower,send_upper);
                const Layout<COORD> outerRange(recv_lower,recv_upper);
                
                if(!outer.contains(innerRange)) Kernel::Layouts::TooManyGhosts(ng,level,Kernel::Layouts::InnerGhost);
                if(!outer.contains(outerRange)) Kernel::Layouts::TooManyGhosts(ng,level,Kernel::Layouts::OuterGhost);

                
                return new Swap(innerRange,outerRange,outer,link.rank);
            }
        };
    }

}

#endif

