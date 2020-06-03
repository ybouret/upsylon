//! \file

#ifndef Y_SPADE_DENSE_MESH_INCLUDED
#define Y_SPADE_DENSE_MESH_INCLUDED 1

#include "y/spade/mesh.hpp"
#include "y/spade/box.hpp"

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
            static const unsigned Space = Coord::Get<COORD>::Dimensions; //!< alias
            Y_DECL_ARGS(T,type);                    //!< aliases
            typedef Kernel::Box<T,Space>    Box;    //!< alias
            typedef typename Box::Vertex    Vertex; //!< alias

            //! cleanup
            inline virtual ~DenseMesh() throw() {}

        protected:
            //! setup
            template <typename LABEL>
            inline explicit DenseMesh(const LABEL         &identifier,
                                      const Layout<COORD> &layout) :
            Mesh<COORD>(identifier,Space),
            Layout<COORD>(layout)
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(DenseMesh);
        };

    }

}

#endif

