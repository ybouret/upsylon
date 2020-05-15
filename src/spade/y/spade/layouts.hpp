
//! \file

#ifndef Y_SPADE_LAYOUTS_INCLUDED
#define Y_SPADE_LAYOUTS_INCLUDED 1

#include "y/spade/layout.hpp"
#include "y/spade/topology.hpp"
#include "y/spade/ghosts.hpp"

namespace upsylon {

    namespace Spade
    {

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
            outer(inner)
            {
                const Coord1D  ng   = abs_of(numGhosts);
                const Node    &self = *this;

                //______________________________________________________________
                //
                // use Node information to expand inner layout
                //______________________________________________________________

                // initialize corners
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
                
                // recompute outer
                new ((void*)&outer) Layout<COORD>(lower,upper);

                //______________________________________________________________
                //
                // compute associated exchange zones in each level
                //______________________________________________________________
                if(ng>0) createAllGhosts(ng);
               
            }


            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const Layout<COORD> inner; //!< inner workspace
            const Layout<COORD> outer; //!< contains exchange zones


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Layouts);
            inline void createAllGhosts(const Coord1D ng)
            {
                assert(ng>0);
                std::cerr << "create ghosts..." << std::endl;
                const Node &self = *this;
                for(unsigned level=0;level<Levels;++level)
                {
                    const Links &links   = self[level];
                    const Link  &forward = links.forward;
                    const Link  &reverse = links.reverse;
                    
                    assert(forward.probe== Topology<COORD>::Coordination::Probes[level]);
                    assert(reverse.probe==-Topology<COORD>::Coordination::Probes[level]);
                    assert(Connect::Authorized(forward.connectMode,reverse.connectMode));
                    
                    switch(links.connect)
                    {
                        case Connect:: FreeStanding: break;
                        case Connect:: AutoExchange:
                            
                            break;
                        case Connect::AsyncTwoWays: break;
                        case Connect::AsyncForward: break;
                        case Connect::AsyncReverse: break;
                        default:
                            Connect::InvalidFlag(links.connect, Connect::Layouts);
                            break;
                    }
                }
            }
            
            inline
            Ghosts *createGhosts(const_coord &probe, const Coord1D ng) const
            {
                assert(ng>0);
                // initialize
                coord innerLo = inner.lower;
                coord innerUp = inner.upper;
                
                coord outerLo = outer.lower;
                coord outerUp = outer.upper;
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    const Coord1D p = Coord::Of(probe,dim);
                    switch(p)
                    {
                        case 1:
                            break;
                            
                        case -1:
                            break;
                            
                        default:
                            assert(0==probe);
                            break;
                    }
                }
                return 0;
            }
        };
    }

}

#endif

