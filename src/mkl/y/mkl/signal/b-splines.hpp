//! \file
#ifndef Y_MKL_B_SPLINES_INCLUDED
#define Y_MKL_B_SPLINES_INCLUDED 1

#include "y/sequence/vector.hpp"
#include "y/type/utils.hpp"
#include "y/mkl/fcn/derivative.hpp"
#include "y/mkl/fcn/integrate.hpp"

namespace upsylon {

    namespace mkl {
        
        //______________________________________________________________________
        //
        //! Return a point on the cubic b-splines: x=0: begin, x=1: end
        //______________________________________________________________________
        template <typename T,typename U>
        inline U Cubic_Bsplines( T x, const array<U> &points ) throw()
        {
            assert(points.size()>0);
            static const T one(1);
            static const T six(6);
            static const T four(4);
            const unit_t num_points   = points.size();
            const unit_t num_segments = num_points + 1;
            unit_t       start_cv     = -2;

            //______________________________________________________________
            //
            // x: 0 -> 1: find the segment
            //______________________________________________________________
            const T      xscaled = clamp<T>(0,x,1)*num_segments;
            const unit_t segment = unit_t(floor_of( xscaled ));
            const T      t       = (x*num_segments-segment);

            //______________________________________________________________
            //
            // prepare points
            //______________________________________________________________
            start_cv += segment;

            const T t2 = t*t;
            const T it = one-t;
            const T t3 = t * t2;

            //______________________________________________________________
            //
            // calculate blending functions for cubic bspline
            //______________________________________________________________
            const T b0 = it*it*it/six;
            const T b1 = (3*t3 - 6*t2 + four)/six;
            const T b2 = (-3*t3 +3*t2 + 3*t + one)/six;
            const T b3 =  t3/six;

            return
            b0 * points[clamp<unit_t>(1,start_cv+1,num_points)] +
            b1 * points[clamp<unit_t>(1,start_cv+2,num_points)] +
            b2 * points[clamp<unit_t>(1,start_cv+3,num_points)] +
            b3 * points[clamp<unit_t>(1,start_cv+4,num_points)];
        }

        //! Cubic B-Spline approximation of a set of vertices
        template<
        typename T,
        template <class> class VTX,
        typename ALLOCATOR = memory::global>
        class CubicApproximation : public vector< VTX<T>, ALLOCATOR>
        {
        public:
            Y_DECL_ARGS(T,type);                             //!< alias
            typedef VTX<type>                     vertex;    //!< alias
            typedef vector<vertex,ALLOCATOR>      base_type; //!< alias
            typedef typename numeric<T>::function func_type; //!< alias
            static const size_t                   DIM = sizeof(vertex)/sizeof(type); //!< dimension

            //! setup
            inline explicit CubicApproximation(const size_t n=0) :
            base_type(n,as_capacity),
            idim(0),
            proj(this, & CubicApproximation::Proj ),
            dlen(this, & CubicApproximation::speed ),
            drvs()
            {}

            //! destructor
            inline virtual ~CubicApproximation() throw() {}

            //! compute the b-spline value
            inline vertex operator()( const T u ) const throw()
            {
                return Cubic_Bsplines(u,*this);
            }

            //! compute the derivative per dimensions
            inline vertex tangent(const T u) const
            {
                // optimal step for u around 0.5
                static const T h_opt = pow_of(numeric<T>::epsilon,T(1.0)/T(3.0))*T(0.5);
                vertex q;
                type *p = (type *)&q;
                for(idim=0;idim<DIM;++idim)
                {
                    p[idim] = drvs.diff(proj,u,h_opt);
                }
                return q;
            }

            //! get local speed 
            inline T speed( const T u ) const
            {
                const vertex v = tangent(u);
                const type  *p = (const type *)&v;
                T ans = square_of(p[0]);
                for(size_t j=1;j<DIM;++j) ans += square_of(p[j]);
                return sqrt_of(ans);
            }

            //! get local length
            inline T arc_length(const T a, const T b ) const
            {
                return integrate::compute(dlen, a, b,
                                         numeric<T>::sqrt_ftol);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(CubicApproximation);
            mutable size_t    idim; //!< for dimension derivative
            mutable func_type proj; //!< coordinate idim
            mutable func_type dlen; //!< speed as function

            inline T Proj( const T u ) const throw()
            {
                assert(idim<DIM);
                const vertex v = (*this)(u);
                const type  *p = (const type *)&v;
                return p[idim];
            }
        public:
            mutable derivative<T> drvs; //!< needed to compute tangent and length

        };
    }
}

#endif
