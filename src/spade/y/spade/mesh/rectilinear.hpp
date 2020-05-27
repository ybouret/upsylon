//! \file

#ifndef Y_SPADE_MESH_RECTILINEAR_INCLUDED
#define Y_SPADE_MESH_RECTILINEAR_INCLUDED 1

#include "y/spade/mesh/dense.hpp"
#include "y/spade/field/in1d.hpp"
#include "y/sequence/slots.hpp"

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
            typedef DenseMesh<COORD,T>        MeshType;
            typedef Layout1D                  AxisLayout;
            typedef Field1D<T>                Axis;
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
                    const string     axisLabel = id + vformat("#%u",dim);
                    axis.template build<const string &,const AxisLayout&>(axisLabel,axisLayout);
                }
            }

            inline virtual const char *category() const throw() { return Category; }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(RectilinearMesh);
            slots<Axis> axis;

        };
        
    }
    
}

#endif


