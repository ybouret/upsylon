
//! \file

#ifndef Y_SPADE_MESH_CURVILINEAR_INCLUDED
#define Y_SPADE_MESH_CURVILINEAR_INCLUDED 1

#include "y/spade/mesh/dense.hpp"

namespace upsylon {
    
    namespace Spade {

        namespace Kernel {

            class Curvilinear
            {
            public:
                static const char Category[];
                virtual ~Curvilinear() throw();


            protected:
                explicit Curvilinear() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Curvilinear);
            };
        }

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


