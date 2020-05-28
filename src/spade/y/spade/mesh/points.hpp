//! \file

#ifndef Y_SPADE_MESH_POINTS_INCLUDED
#define Y_SPADE_MESH_POINTS_INCLUDED 1

#include "y/spade/mesh/loose.hpp"

namespace upsylon {

    namespace Spade {

        namespace Kernel {

            //! common stuff for point meshes
            class PointMesh
            {
            public:
                virtual ~PointMesh() throw(); //!< cleanup
                static const char Category[]; //!< "points"
                
            protected:
                explicit PointMesh() throw(); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(PointMesh);
            };
        }

        //----------------------------------------------------------------------
        //
        //! point mesh
        //
        //----------------------------------------------------------------------
        template <typename COORD, unsigned SPACE, typename T>
        class PointMesh :
        public Kernel::PointMesh,
        public LooseMesh<COORD,SPACE,T>
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef LooseMesh<COORD,SPACE,T>    MeshType; //!< alias
            typedef typename MeshType::Vertex   Vertex;   //!< alias
            typedef typename MeshType::Vertices Vertices; //!< alias

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------

            //! cleanup
            inline virtual ~PointMesh() throw() {}

            //! setup
            template <typename LABEL>
            inline explicit PointMesh(const LABEL         &identifier,
                                      const Layout<COORD> &layout) :
            MeshType(identifier,layout)
            {
            }
            

            virtual const char *category() const throw() { return Category; }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(PointMesh);
        };

    }

}

#endif
