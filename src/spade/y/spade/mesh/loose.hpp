
//! \file

#ifndef Y_SPADE_LOOSE_MESH_INCLUDED
#define Y_SPADE_LOOSE_MESH_INCLUDED 1

#include "y/spade/mesh.hpp"

namespace upsylon {

    namespace Spade {
        
        //------------------------------------------------------------------
        //
        //! a loose mesh: topology<=space
        //
        //------------------------------------------------------------------
        template <
        typename COORD,
        unsigned SPACE,
        typename T>
        class LooseMesh : public Mesh<COORD>
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            Y_DECL_ARGS(T,type);                                            //!< aliases
            typedef typename VertexIn<SPACE>:: template Of<mutable_type>::Type Vertex; //!< alias
            static const size_t Space = SPACE;                              //!< alias
            //! cleanup
            inline virtual ~LooseMesh() throw() {}

        protected:
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LooseMesh);
        };

    }

}

#endif

