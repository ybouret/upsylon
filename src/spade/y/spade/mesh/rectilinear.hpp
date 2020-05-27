//! \file

#ifndef Y_SPADE_MESH_RECTILINEAR_INCLUDED
#define Y_SPADE_MESH_RECTILINEAR_INCLUDED 1

#include "y/spade/mesh.hpp"

namespace upsylon {
    
    namespace Spade {
        
        template <typename COORD>
        class RectilinearMesh :
        public Mesh<COORD>
        {
        public:
            typedef Mesh<COORD> MeshType;
            typedef Layout1D    AxisLayout;

            inline virtual ~RectilinearMesh() throw()
            {
            }


            template <typename LABEL> inline
            explicit RectilinearMesh(const LABEL         &id,
                                     const Layout<COORD> &) :
            MeshType(id,1)
            {
            }

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(RectilinearMesh);
        };
        
    }
    
}

#endif


