
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
            typedef typename Layout<COORD>::coord       coord;
            typedef typename Layout<COORD>::const_coord const_coord;

            typedef typename Topology<COORD> ::Node            Node;
            typedef typename Topology<COORD> ::Boolean         Boolean;

            const Layout<COORD> inner; //!< workspace
            const Layout<COORD> outer; //!< exchange zones

            inline virtual ~Layouts() throw()
            {
            }

            inline explicit Layouts(const Layout<COORD>   &full,
                                    const_coord            rnks,
                                    const Topology<COORD> &topo,
                                    const Boolean         &pbcs) :
            Node(rnks,topo,pbcs),
            inner(full.split(topo.sizes,this->ranks)),
            outer(inner)
            {
            }
            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Layouts);
        };
    }

}

#endif

