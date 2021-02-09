
//! \file

#ifndef Y_GFX_TOPOLOGY_INCLUDED
#define Y_GFX_TOPOLOGY_INCLUDED 1

#include "y/gfx/area/boundaries.hpp"
#include "y/gfx/area/core.hpp"


namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! Topology is an area with core and boudaries
        //
        //______________________________________________________________________
        class Topology :
        public Object,
        public Area,
        public Core,
        public Boundaries
        {
        public:
            typedef arc_ptr<const Topology> Pointer; //!< alias

            explicit Topology(const Area &area); //!< setup
            virtual ~Topology() throw();         //!< cleanup
            
            //! show
            friend std::ostream & operator<<(std::ostream &os, const Topology &topo);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Topology);
        };

    }

}

#endif
