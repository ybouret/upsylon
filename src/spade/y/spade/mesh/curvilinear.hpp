
//! \file

#ifndef Y_SPADE_MESH_CURVILINEAR_INCLUDED
#define Y_SPADE_MESH_CURVILINEAR_INCLUDED 1

#include "y/spade/mesh/dense.hpp"
#include "y/spade/fields.hpp"
#include "y/sequence/slots.hpp"
#include "y/spade/box.hpp"
#include "y/mkl/types.hpp"

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
                
                //! check valid radii
                static void CheckTorus(const double rOut, const double rIn);

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
            typedef typename MeshType::Box    Box;       //!< alias

            typedef typename FieldFor<COORD> ::
            template Of<mutable_type>::Type   Axis;       //!< alias
            typedef arc_ptr<Axis>             AxisHandle; //!< alias for dynamic axis

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
                    const string     axisString = id + this->AxisTag(dim);
                    const AxisHandle axisHandle = new Axis(axisString,*this);
                    axis.push(axisHandle);
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
                return *axis[dim];
            }
            
            //! axis access, const
            inline const Axis & operator[](const unsigned dim) const throw()
            {
                assert(dim<Dimensions);
                return *axis[dim];
            }
            
            //! recompose vertex at coordinate
            inline Vertex operator()(const COORD C) const throw()
            {
                Vertex        v(0);
                mutable_type *p = (mutable_type *)&v;
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    p[dim] = (*axis[dim])[C];
                }
                return v;
            }
            
            //! set vertex at coordinate
            inline void operator()(const COORD C, const Vertex v) throw()
            {
                const_type *p = (const_type *)&v;
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    (*axis[dim])[ C ] = p[dim];
                }
            }

            //! mapping to a box on an original layout
            inline void mapRegular(const Box           &box,
                                   const Layout<COORD> &layout)
            {
                assert(this->isThick());
                CurvilinearMesh &            self = *this;
                typename Layout<COORD>::Loop loop(this->lower,this->upper);
                Vertex                       v;
                const COORD                 &i  = loop.value;
                const_type                  *l  = box._lower();
                const_type                  *w  = box._width();
                mutable_type                *p  = (mutable_type *)&v;
                const COORD                  d  = layout.width - Coord::Ones<COORD>();
                for(loop.boot();loop.good();loop.next())
                {
                    const COORD j = i - layout.lower;
                    for(unsigned dim=0;dim<Dimensions;++dim)
                    {
                        p[dim] = l[dim] + (Coord::Of(j,dim) * w[dim])/Coord::Of(d,dim);
                    }
                    self(i,v);
                }
            }

            //! mapping to a 3D torus
            void mapTorus(const_type rOut, const_type rIn )
            {
                //static const_type two_pi = mkl::numeric<T>::two_pi;
                assert(this->isThick());
                CurvilinearMesh &self = *this;
                self.CheckTorus(rOut,rIn);

#if 0
                // first coordinate: describe out
                const_type dThetaOut = two_pi / this->width.x;
                for(Coord1D i=this->lower.x;i<=this->upper.x;++i)
                {
                    const_type thetaOut = ( (i-this->lower.x) * dThetaOut );
                    const_type x        = rOut * mkl::cos_of( thetaOut );
                    const_type y        = rOut * mkl::sin_of( thetaOut );
                }
#endif

            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(CurvilinearMesh);
            slots<AxisHandle> axis;
        };

    }
    
}

#endif


