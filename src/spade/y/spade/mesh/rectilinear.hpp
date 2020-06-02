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

            //! common stuff for rectilinear meshes
            class Rectilinear
            {
            public:
                static const char Category[];   //!< "rectilinear"
                virtual ~Rectilinear() throw(); //!< cleanup
                
            protected:
                explicit Rectilinear() throw(); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rectilinear);
            };
        }

        //----------------------------------------------------------------------
        //
        //! rectilinear mesh
        /**
         - one 1D field per dimensions
         - topology = space = Dimensions
         */
        //
        //----------------------------------------------------------------------
        template <typename COORD, typename T>
        class RectilinearMesh : public Kernel::Rectilinear, public DenseMesh<COORD,T>
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            Y_DECL_ARGS(T,type);                          //!< alias
            typedef DenseMesh<COORD,T>        MeshType;   //!< alias
            typedef Layout1D                  AxisLayout; //!< alias
            typedef Field1D<mutable_type>     Axis;       //!< alias
            typedef typename MeshType::Vertex Vertex;     //!< alias
            //! alias
            static  const unsigned            Dimensions = MeshType::Dimensions;

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            inline virtual ~RectilinearMesh() throw()
            {}

            //! setup
            template <typename LABEL> inline
            explicit RectilinearMesh(const LABEL         &id,
                                     const Layout<COORD> &full) :
            MeshType(id,full), axis(Dimensions)
            {
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    const AxisLayout axisLayout(Coord::Of(this->lower,dim),Coord::Of(this->upper,dim));
                    const string     axisLabel = id + this->AxisTag(dim);
                    axis.template build<const string &,const AxisLayout&>(axisLabel,axisLayout);
                }
            }

            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            
            //! return category
            inline virtual const char *category() const throw() { return Category; }

            //! axis access
            inline Axis & operator[](const unsigned dim) throw()
            {
                assert(dim<Dimensions);
                return axis[dim];
            }
            
            //! axis acces, CONST
            inline const Axis & operator[](const unsigned dim) const throw()
            {
                assert(dim<Dimensions);
                return axis[dim];
            }
            
            //! recompose vertex from coordinate
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
            
            //! set vertex at coordinate
            inline void operator()(const COORD C, const Vertex v) throw()
            {
                const_type *p = (const_type *)&v;
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    axis[dim][ Coord::Of(C,dim) ] = v[dim];
                }
            }
            
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(RectilinearMesh);
            slots<Axis> axis;
        };
        
    }
    
}

#endif


