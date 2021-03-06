
//! \file

#ifndef Y_SPADE_MESH_CURVILINEAR_INCLUDED
#define Y_SPADE_MESH_CURVILINEAR_INCLUDED 1

#include "y/spade/mesh/dense.hpp"
#include "y/spade/field/ops.hpp"
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
            Y_DECL_ARGS(T,type);                            //!< aliases
            typedef DenseMesh<COORD,T>           MeshType;  //!< alias
            typedef typename MeshType::Vertex    Vertex;    //!< alias
            typedef typename MeshType::Box       Box;       //!< alias

            typedef typename FieldFor<COORD> ::
            template Of<mutable_type>::Type      Axis;       //!< alias
            typedef arc_ptr<Axis>                AxisHandle; //!< alias for dynamic axis
            typedef typename Layout<COORD>::Loop Loop;       //!< alias for loops

            
            //! alias
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

            //------------------------------------------------------------------
            //
            // mapping
            //
            //------------------------------------------------------------------
            
            
            //! mapping to a box on an original layout
            inline void mapRegular(const Box           &box,
                                   const Layout<COORD> &layout)
            {
                assert(this->isThick());
                CurvilinearMesh & self = *this;
                Loop              loop(self.lower,self.upper);
                Vertex            v;
                const COORD      &i  = loop.value;
                const_type       *l  = box._lower();
                const_type       *w  = box._width();
                mutable_type     *p  = (mutable_type *)&v;
                const COORD       d  = layout.width - Coord::Ones<COORD>();
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
            
            //! map a coordinate to annulus
            inline void mapAnnulus(const_type R1, const_type R2)
            {
                static const_type two_pi = mkl::numeric<mutable_type>::two_pi;
                assert(R2>R1);
                assert(this->lower.x<this->upper.x);
                const_type        delta  = R2-R1;
                const_type        delta2 = delta*delta;
                CurvilinearMesh & self   = *this;
                const COORD       lo     = self.lower;
                const COORD       up     = self.upper;
                Axis             &X      = self[0];
                Axis             &Y      = self[1];
                for(Coord1D i=lo.x;i<=up.x;++i)
                {
                    const_type r = R1 + mkl::sqrt_of( (i-lo.x) * delta2 / (up.x-lo.x) );
                    for(Coord1D j=lo.y;j<=up.y;++j)
                    {
                        const_type theta = (j-lo.y) * two_pi / (1+up.y-lo.y);
                        const_type x     = r * mkl::cos_of(theta);
                        const_type y     = r * mkl::sin_of(theta);
                        X[j][i] = x;
                        Y[j][i] = y;
                    }
                }
                
            }
            
            //------------------------------------------------------------------
            //
            // properties
            //
            //------------------------------------------------------------------
            
            //! get the Axis Aligned Bounding Box
            inline Box aabb() const
            {
                Vertex lo(0);
                Vertex up(0);
                type  *l = (type *) &lo;
                type  *u = (type *) &up;
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    const Axis &A = *axis[dim];
                    Ops::MinMax(l[dim], u[dim],A,A);
                }
                return Box(lo,up);
            }
            
            //! compute barycenter
            inline Vertex barycenter() const
            {
                const CurvilinearMesh &self = *this;
                Loop                   loop(self.lower,self.upper);
                Vertex bar(0);
                for(loop.boot();loop.good();loop.next())
                {
                    const Vertex v = self(*loop);
                    bar += v;
                }
                bar /= mutable_type(self.items);
                return bar;
            }
            
            //! gyration radius
            inline mutable_type Rg(Vertex &bar) const
            {
                const CurvilinearMesh &self = *this;
                Loop                   loop(self.lower,self.upper);
                
                bar = barycenter();
                mutable_type r2 = 0;
                for(loop.boot();loop.good();loop.next())
                {
                    const Vertex delta = self(*loop) - bar;
                    r2 += __point::norm2(delta);
                }
                r2 /= mutable_type(self.items);
                return mkl::sqrt_of(r2);
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(CurvilinearMesh);
            slots<AxisHandle> axis;
        };

    }
    
}

#endif


