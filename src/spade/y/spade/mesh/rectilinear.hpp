//! \file

#ifndef Y_SPADE_MESH_RECTILINEAR_INCLUDED
#define Y_SPADE_MESH_RECTILINEAR_INCLUDED 1

#include "y/spade/mesh.hpp"

namespace upsylon {
    
    namespace Spade {
        
        template <typename COORD>
        class RectilinearMesh : public MeshOf<COORD>
        {
        public:
            typedef Layout1D AxisLayout;
            
            explicit RectilinearMesh( const Layout<COORD> &L );
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(RectilinearMesh);
        };
        
    }
    
}

#endif


