
//! \file

#ifndef Y_GFX_TOPOLOGY_INCLUDED
#define Y_GFX_TOPOLOGY_INCLUDED 1

#include "y/gfx/sub-area.hpp"


namespace upsylon
{
    namespace GFX
    {
        class Topology : public Object, public Area
        {
        public:

            explicit Topology(const Area &area);
            virtual ~Topology() throw();
            Topology(const Topology &); //!< copy

            const AreaHandle core;
            


        private:
            Y_DISABLE_ASSIGN(Topology);

        };
    }

}

#endif
