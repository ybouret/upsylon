
//! \file

#ifndef Y_SPADE_LOOSE_MESH_INCLUDED
#define Y_SPADE_LOOSE_MESH_INCLUDED 1

#include "y/spade/mesh.hpp"

namespace upsylon {

    namespace Spade {

        //! a loose mesh: topology==dimensions
        template <typename COORD,typename T>
        class LooseMesh : public Mesh<COORD>
        {
        public:
            Y_DECL_ARGS(T,type);

            typedef typename VertexFor<COORD>:: template Of<T>::Type Vertex;

            inline virtual ~LooseMesh() throw() {}

        protected:


        private:
            Y_DISABLE_COPY_AND_ASSIGN(LooseMesh);
        };

    }

}

#endif

