
//! \file

#ifndef Y_SPADE_MESH_CURVILINEAR_INCLUDED
#define Y_SPADE_MESH_CURVILINEAR_INCLUDED 1

#include "y/spade/mesh/dense.hpp"
#include "y/spade/fields.hpp"
#include "y/sequence/slots.hpp"

namespace upsylon {
    
    namespace Spade {

        namespace Kernel {

            //! common stuff for curvilinear meshes
            class Curvilinear
            {
            public:
                static const char Category[];   //!< "curvilinear"
                virtual ~Curvilinear() throw(); //!< cleanup

            protected:
                explicit Curvilinear() throw(); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Curvilinear);
            };
        }

        //----------------------------------------------------------------------
        //
        //! rectilinear mesh
        /**
         - one ND field per dimensions
         - topology = space = Dimensions
         */
        //
        //----------------------------------------------------------------------
        template <typename COORD, typename T>
        class CurvilinearMesh : public Kernel::Curvilinear, public DenseMesh<COORD,T>
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            Y_DECL_ARGS(T,type);                         //!< aliases
            typedef DenseMesh<COORD,T>        MeshType;  //!< alias
            typedef typename MeshType::Vertex Vertex;    //!< alias
            typedef typename FieldFor<COORD> :: template Of<mutable_type>::Type Axis; //!< alias
            //! alas
            static  const unsigned            Dimensions = MeshType::Dimensions;

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            inline virtual ~CurvilinearMesh() throw()
            {
            }

            //! setup
            template <typename LABEL> inline
            explicit CurvilinearMesh(const LABEL         &id,
                                     const Layout<COORD> &L) :
            MeshType(id,L),
            axis(Dimensions)
            {
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    const string axisLabel = id + this->AxisTag(dim);
                    axis. template build<const string&,const Layout<COORD> &>(id,*this);
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
            
            //! axis access, const
            inline const Axis & operator[](const unsigned dim) const throw()
            {
                assert(dim<Dimensions);
                return axis[dim];
            }
            
            //! recompose vertex at coordinate
            inline Vertex operator()(const COORD C) const throw()
            {
                Vertex        v(0);
                mutable_type *p = (mutable_type *)&v;
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    p[dim] = axis[dim][C];
                }
                return v;
            }
            
            //! set vertex at coordinate
            inline void operator()(const COORD C, const Vertex v) throw()
            {
                const_type *p = (const_type *)&v;
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    axis[dim][ C ] = p[dim];
                }
            }
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(CurvilinearMesh);
            slots<Axis> axis;
        };

    }
    
}

#endif


