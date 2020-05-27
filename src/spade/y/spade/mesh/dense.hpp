//! \file

#ifndef Y_SPADE_DENSE_MESH_INCLUDED
#define Y_SPADE_DENSE_MESH_INCLUDED 1

#include "y/spade/mesh.hpp"
#include "y/spade/vertices.hpp"

namespace upsylon {

    namespace Spade {
        
        //----------------------------------------------------------------------
        //
        //! a dense mesh: topology==space
        //
        //----------------------------------------------------------------------
        template <typename COORD,typename T>
        class DenseMesh : public Mesh<COORD>, public Layout<COORD>
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static const unsigned Dimensions = Coord::Get<COORD>::Dimensions;           //!< alias
            Y_DECL_ARGS(T,type);                                                        //!< aliases
            typedef typename VertexFor<COORD>:: template Of<mutable_type>::Type Vertex; //!< alias

            //! cleanup
            inline virtual ~DenseMesh() throw() {}

        protected:
            //! setup
            template <typename LABEL>
            inline explicit DenseMesh(const LABEL         &id,
                                      const Layout<COORD> &full) :
            Mesh<COORD>(id,Dimensions),
            Layout<COORD>(full)
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DenseMesh);
        };

    }

}

#endif

