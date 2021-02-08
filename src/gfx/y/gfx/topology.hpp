
//! \file

#ifndef Y_GFX_TOPOLOGY_INCLUDED
#define Y_GFX_TOPOLOGY_INCLUDED 1

#include "y/gfx/borders.hpp"
#include "y/gfx/corners.hpp"


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
        class Topology : public Object, public Area
        {
        public:
            typedef arc_ptr<const Topology> Pointer;

            explicit Topology(const Area &area);
            virtual ~Topology() throw();

            const SubAreaHandle core;     //!< optional core
            const unit_t        inside;   //!< #core
            const Borders       borders;  //!< all borders
            const Corners       corners;  //!< all corners

            friend std::ostream & operator<<(std::ostream &os, const Topology &topo);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Topology);
        };

    }

}

#endif
