
//! \file

#ifndef Y_GFX_TOPOLOGY_INCLUDED
#define Y_GFX_TOPOLOGY_INCLUDED 1

#include "y/gfx/area/boundaries.hpp"


namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! Topology is an area with core, borders and corners
        //
        //______________________________________________________________________
        class Topology :
        public Object,
        public Boundaries,
        public Area
        {
        public:
            typedef arc_ptr<const Topology> Pointer; //!< alias

            
            explicit Topology(const Area &area); //!< setup
            virtual ~Topology() throw();         //!< cleanup

            const SubArea::Handle core;   //!< optional core
            const unit_t          inner;  //!< in core
            const unit_t          outer;  //!< outside core
            
            //! show
            friend std::ostream & operator<<(std::ostream &os, const Topology &topo);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Topology);
        };

    }

}

#endif
