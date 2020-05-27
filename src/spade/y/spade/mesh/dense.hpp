//! \file

#ifndef Y_SPADE_DENSE_MESH_INCLUDED
#define Y_SPADE_DENSE_MESH_INCLUDED 1

#include "y/spade/mesh.hpp"
#include "y/spade/vertices.hpp"

namespace upsylon {

    namespace Spade {
        
        //! a dense mesh: topology==dimensions
        template <typename COORD,typename T>
        class DenseMesh : public Mesh<COORD>
        {
        public:
            static const unsigned Dimensions = Coord::Get<COORD>::Dimensions;
            Y_DECL_ARGS(T,type);

            typedef typename VertexFor<COORD>:: template Of<T>::Type Vertex;

            inline virtual ~DenseMesh() throw() {}

        protected:
            template <typename LABEL>
            inline explicit DenseMesh(const LABEL &id) : Mesh<COORD>(id)
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DenseMesh);
        };

    }

}

#endif

