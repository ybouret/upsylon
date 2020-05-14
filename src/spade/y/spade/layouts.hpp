
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
            static const unsigned Dimensions = Layout<COORD>::Dimensions;
            typedef typename Layout<COORD>::coord       coord;
            typedef typename Layout<COORD>::const_coord const_coord;

            typedef typename Topology<COORD>::Node            Node;
            typedef typename Topology<COORD>::Link            Link;
            typedef typename Topology<COORD>::Links           Links;
            typedef typename Topology<COORD>::Boolean         Boolean;

            const Layout<COORD> inner; //!< workspace
            const Layout<COORD> outer; //!< exchange zones

            inline virtual ~Layouts() throw()
            {
            }

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
                
                // use Node information to expand inner layout
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
                            std::cerr << "increase " << ng << " along " << links.forward.probe << std::endl;
                            Coord::Of(upper,dim) += ng;
                    }
                    
                    switch(links.reverse.connectMode)
                    {
                        case Connect::Zilch: break;
                        case Connect::Async:
                        case Connect::Local:
                            std::cerr << "decrease " << ng << " along " << links.reverse.probe << std::endl;
                            Coord::Of(lower,dim) -= ng;
                    }
                }
                
                // recompute outer
                new ((void*)&outer) Layout<COORD>(lower,upper);
            }
            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Layouts);
        };
    }

}

#endif

