//! \file

#ifndef Y_SPADE_MESH_RECTILINEAR_INCLUDED
#define Y_SPADE_MESH_RECTILINEAR_INCLUDED 1

#include "y/spade/mesh/dense.hpp"
#include "y/spade/field/in1d.hpp"
#include "y/sequence/slots.hpp"
#include "y/spade/vertices.hpp"

namespace upsylon {
    
    namespace Spade {

        namespace Kernel {

            class Rectilinear
            {
            public:
                static const char Category[];
                virtual ~Rectilinear() throw();


            protected:
                explicit Rectilinear() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rectilinear);
            };
        }

        template <typename COORD, typename T>
        class RectilinearMesh :
        public Kernel::Rectilinear,
        public DenseMesh<COORD,T>
        {
        public:
            Y_DECL_ARGS(T,type);
            typedef DenseMesh<COORD,T>        MeshType;
            typedef Layout1D                  AxisLayout;
            typedef Field1D<T>                Axis;
            typedef VertexFor<COORD>          _VTX;
            typedef typename _VTX:: template Of<T>::Type Vertex;
            static  const unsigned            Dimensions = MeshType::Dimensions;

            inline virtual ~RectilinearMesh() throw()
            {
            }


            template <typename LABEL> inline
            explicit RectilinearMesh(const LABEL         &id,
                                     const Layout<COORD> &full) :
            MeshType(id,full),
            axis( Dimensions )
            {
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    const AxisLayout axisLayout(Coord::Of(this->lower,dim),Coord::Of(this->upper,dim));
                    const string     axisLabel = id + this->AxisTag(dim);
                    axis.template build<const string &,const AxisLayout&>(axisLabel,axisLayout);
                }
            }

            inline virtual const char *category() const throw() { return Category; }

            inline Axis & operator[](const unsigned dim) throw()
            {
                assert(dim<Dimensions);
                return axis[dim];
            }
            
            inline const Axis & operator[](const unsigned dim) const throw()
            {
                assert(dim<Dimensions);
                return axis[dim];
            }
            
            inline Vertex operator()(const COORD C) const throw()
            {
                Vertex        v(0);
                mutable_type *p = (mutable_type *)&v;
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    p[dim] = axis[dim][ Coord::Of(C,dim) ];
                }
                return v;
            }
            
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RectilinearMesh);
            slots<Axis> axis;

        };
        
    }
    
}

#endif


