
//! \file

#ifndef Y_SPADE_LAYOUTS_INCLUDED
#define Y_SPADE_LAYOUTS_INCLUDED 1

#include "y/spade/layout.hpp"
#include "y/spade/topology.hpp"

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
                coord lower = inner.lower;
                coord upper = inner.upper;
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
        };
    }

}

#endif

