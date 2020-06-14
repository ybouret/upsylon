
//! \file

#ifndef Y_SPADE_LOOSE_MESH_INCLUDED
#define Y_SPADE_LOOSE_MESH_INCLUDED 1

#include "y/spade/mesh.hpp"
#include "y/spade/fields.hpp"
#include "y/spade/vertices.hpp"
#include "y/spade/box.hpp"

namespace upsylon {

    namespace Spade {

        namespace Kernel
        {
            //! common stuff, mostly to compute types
            template <typename COORD,unsigned SPACE,typename T>
            struct Loose
            {
                typedef typename VertexIn<SPACE>:: template Of<T>::Type       Vertex;   //!< alias
                typedef typename FieldFor<COORD>:: template Of<Vertex> ::Type Vertices; //!< alias
            };
        };
        
        //------------------------------------------------------------------
        //
        //! a loose mesh: topology<=space
        //
        //------------------------------------------------------------------
        template <typename COORD, unsigned SPACE, typename T>
        class LooseMesh :
        public Mesh<COORD>,
        public Kernel::Loose<COORD,SPACE,T>::Vertices
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef typename Kernel::Loose<COORD,SPACE,T>::Vertices Vertices; //!< alias
            typedef typename Kernel::Loose<COORD,SPACE,T>::Vertex   Vertex;   //!< alias
            static  const    size_t Space =                         SPACE;    //!< alias
            typedef Kernel::Box<T,Space>                            Box;      //!< alias

            //! cleanup
            inline virtual ~LooseMesh() throw() {}

        protected:
            //! setyup
            template <typename LABEL>
            explicit LooseMesh(const LABEL         &identifier,
                               const Layout<COORD> &layout) :
            Mesh<COORD>(identifier,Space),
            Vertices(identifier,layout)
            {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(LooseMesh);
        };

    }

}

#endif

