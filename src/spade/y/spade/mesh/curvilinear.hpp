
//! \file

#ifndef Y_SPADE_MESH_CURVILINEAR_INCLUDED
#define Y_SPADE_MESH_CURVILINEAR_INCLUDED 1

#include "y/spade/mesh.hpp"

namespace upsylon {
    
    namespace Spade {

        template <typename COORD>
        class CurvilinearMesh :
        public Mesh<COORD>
        {
        public:
            typedef Mesh<COORD>   MeshType;
            static const unsigned Dimensions = MeshType::Dimensions;
            typedef Layout1D      AxisLayout;

            inline virtual ~CurvilinearMesh() throw()
            {
            }

            template <typename LABEL> inline
            explicit CurvilinearMesh(const LABEL         &id,
                                     const Layout<COORD> &) :
            MeshType(id,Dimensions)
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(CurvilinearMesh);
        };

    }
    
}

#endif


