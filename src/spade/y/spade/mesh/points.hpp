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
                static const char Category[]; //!< "point"
                
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
            typedef LooseMesh<COORD,SPACE,T>     MeshType; //!< alias
            typedef typename MeshType::Vertex    Vertex;   //!< alias
            typedef typename MeshType::Vertices  Vertices; //!< alias
            typedef typename MeshType::Box       Box;      //!< alias
            typedef typename Layout<COORD>::Loop Loop;     //!< alias
            Y_DECL_ARGS(T,type);                           //!< aliases

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

            //! get the Axis Aligned Bounding Box
            inline Box aabb() const
            {
                const PointMesh &self = *this;
                Loop             loop(self.lower,self.upper); loop.boot();
                Vertex           lo = self[ *loop ];
                Vertex           up = lo;
                mutable_type    *l = (mutable_type *)&lo;
                mutable_type    *u = (mutable_type *)&up;

                for( loop.next(); loop.good(); loop.next() )
                {
                    const Vertex temp = self[ *loop ];
                    const_type  *v    = (const_type *) &temp;
                    for(unsigned dim=0;dim<SPACE;++dim)
                    {
                        const_type value = v[dim];
                        l[dim] = min_of(l[dim],value);
                        u[dim] = max_of(u[dim],value);
                    }
                }
                return Box(lo,up);
            }
            
            //! get the barycenter
            inline Vertex barycenter() const
            {
                const PointMesh &self = *this;
                Loop             loop(self.lower,self.upper); loop.boot();
                
                Vertex G(0);
                for( loop.next(); loop.good(); loop.next() )
                {
                    G += self[ *loop ];
                }
                G /= self.items;
                return G;
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(PointMesh);
        };

    }

}

#endif
