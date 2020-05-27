//! \file

#ifndef Y_SPADE_MESH_RECTILINEAR_INCLUDED
#define Y_SPADE_MESH_RECTILINEAR_INCLUDED 1

#include "y/spade/mesh/dense.hpp"

namespace upsylon {
    
    namespace Spade {



        template <typename COORD, typename T>
        class RectilinearMesh : public DenseMesh<COORD,T>
        {
        public:
            typedef DenseMesh<COORD,T> MeshType;
            typedef Layout1D           AxisLayout;
            static  const unsigned     Dimensions = MeshType::Dimensions;

            inline virtual ~RectilinearMesh() throw()
            {
            }


            template <typename LABEL> inline
            explicit RectilinearMesh(const LABEL         &id,
                                     const Layout<COORD> &) :
            MeshType(id)
            {
            }

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(RectilinearMesh);
        };
        
    }
    
}

#endif


