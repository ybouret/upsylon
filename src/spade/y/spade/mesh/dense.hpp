//! \file

#ifndef Y_SPADE_DENSE_MESH_INCLUDED
#define Y_SPADE_DENSE_MESH_INCLUDED 1

#include "y/spade/mesh.hpp"

namespace upsylon {

    namespace Spade {
        
        //! a dense mesh: topology==dimensions
        template <typename COORD,typename T>
        class DenseMesh : public Mesh<COORD>, public Layout<COORD>
        {
        public:
            static const unsigned Dimensions = Coord::Get<COORD>::Dimensions;
            Y_DECL_ARGS(T,type);

            
            inline virtual ~DenseMesh() throw() {}

        protected:
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

