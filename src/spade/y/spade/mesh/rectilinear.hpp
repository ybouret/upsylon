//! \file

#ifndef Y_SPADE_MESH_RECTILINEAR_INCLUDED
#define Y_SPADE_MESH_RECTILINEAR_INCLUDED 1

#include "y/spade/mesh/dense.hpp"
#include "y/spade/field/in1d.hpp"
#include "y/spade/field/ops.hpp"
#include "y/sequence/slots.hpp"

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
            Y_DECL_ARGS(T,type);                             //!< alias
            typedef DenseMesh<COORD,T>           MeshType;   //!< alias
            typedef Layout1D                     AxisLayout; //!< alias
            typedef Field1D<mutable_type>        Axis;       //!< alias
            typedef typename MeshType::Vertex    Vertex;     //!< alias
            typedef typename MeshType::Box       Box;        //!< alias
            typedef typename Layout<COORD>::Loop Loop;       //!< alias
            
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

            //! mapping to a box on an original layout
            inline void mapRegular(const Box           &box,
                                   const Layout<COORD> &layout) throw()
            {
                assert(this->isThick());
                const_type *l = box._lower();
                const_type *u = box._upper();
                const_type *w = box._width();
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    Axis          &a      = axis[dim];
                    const Coord1D  thisL = a.lower;
                    const Coord1D  thisU = a.upper;
                    const Coord1D  fullL = Coord::Of(layout.lower,dim);
                    const Coord1D  fullU = Coord::Of(layout.upper,dim);
                    const Coord1D  fullW = Coord::Of(layout.width,dim);
                    const Coord1D  den   = fullW-1; assert(den>0);
                    const_type     L     = l[dim];
                    const_type     W     = w[dim];
                    const_type     U     = u[dim];
                    for(Coord1D i=thisL;i<=thisU;++i)
                    {
                        a[i] = L + (W*(i-fullL))/den; // TODO: correct it...
                    }

                    if(a.has(fullL))
                    {
                        a[fullL] = L;
                    }

                    if(a.has(fullU))
                    {
                        a[fullU] = U;
                    }
                }
            }
            
            //! get the Axis Aligned Bounding Box
            inline Box aabb() const
            {
                Vertex lo(0);
                Vertex up(0);
                type  *l = (type *) &lo;
                type  *u = (type *) &up;
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    const Axis &_ = axis[dim];
                    Ops::MinMax(l[dim],u[dim],_,_);
                }
                return Box(lo,up);
            }

            //! compute barycenter
            inline Vertex barycenter() const
            {
                mutable_type bar[4] = { 0,0,0,0 };
                for(unsigned dim=0;dim<Dimensions;++dim)
                {
                    const Axis   &a = axis[dim];
                    mutable_type &g = bar[dim];
                    for(Coord1D i=a.lower;i<=a.upper;++i)
                    {
                        g += a[i];
                    }
                    g/= mutable_type(a.width);
                }
                return *(Vertex *)&bar[0];
            }
            
            //! radius
            inline mutable_type Rg(Vertex &bar) const
            {
                const RectilinearMesh &self = *this;
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
            Y_DISABLE_COPY_AND_ASSIGN(RectilinearMesh);
            slots<Axis> axis;
        };
        
    }
    
}

#endif


