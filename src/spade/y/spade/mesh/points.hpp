
/! \file

#ifndef Y_SPADE_MESH_POINTS_INCLUDED
#define Y_SPADE_MESH_POINTS_INCLUDED 1

#include "y/spade/mesh/loose.hpp"

namespace upsylon {

    namespace Spade {

        namespace Kernel
        {
            class PointMesh
            {
            public:

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rectilinear);
            };
        }

        template <typename COORD, typename T>
        class PointMesh : public LooseMesh<COORD,T>
        {
        public:

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PointMesh);
        };

    }

}

#endif
