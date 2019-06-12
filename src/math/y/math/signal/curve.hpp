//! \file

#ifndef Y_SIGNAL_CURVE_INCLUDED
#define Y_SIGNAL_CURVE_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/math/kernel/tridiag.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace math
    {
        //! 1D/2D/3D... spline
        struct curve
        {
            //! template to extract info from types
            template <typename T> struct info_for;



            //! for boundaries
            enum style
            {
                standard, //!< standard spline
                periodic  //!< periodic spline
            };

            template <class> class spline; //!< forward declaration

            //! interface class for run-time info
            class interface
            {
            public:
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const size_t   dimensions; //!< number of components
                const size_t   real_bytes; //!< sizeof(real)
                const size_t   point_size; //!< dimensions * real_bytes
                const bool     computed;   //!< status flag

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual ~interface() throw();                                    //!< destructor
                virtual size_t      size()                   const throw() = 0;  //!< number of points
                virtual const void *addr(const size_t index) const throw() = 0;  //!< address of one point

                //______________________________________________________________
                //
                // non virtual interface
                //______________________________________________________________
                void save_point( ios::ostream &fp, const void *p ) const;       //!< save a point
                void save( ios::ostream &fp ) const;                            //!< save point_index point.x [point.y...]

            protected:
                explicit interface(const size_t d, const size_t r) throw();     //!< setup
                void set_computed(bool flag) throw();                           //!< change status

            private:
                Y_DISABLE_COPY_AND_ASSIGN(interface);
            };

            //! template interface
            template <typename T>
            class interface_for : public interface
            {
            public:
                inline virtual ~interface_for() throw() {} //!< destructor
                
            protected:
                //! constructor
                explicit interface_for(const size_t d ) throw() : interface(d,sizeof(T)) {}
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(interface_for);
            };


            //! points to build spline, with delta_t = 1 for computations
            template <
            typename POINT,
            typename ALLOCATOR= memory::global
            >
            class points : public interface_for< typename info_for<POINT>::real >
            {
            public:
                //______________________________________________________________
                //
                // definitions
                //______________________________________________________________
                typedef info_for<POINT>          info_type;            //!< alias
                static const size_t              dim = info_type::dim; //!< dimension
                typedef POINT                    point;                //!< alias
                typedef typename info_type::real real;                 //!< alias
                typedef vector<point,ALLOCATOR>  vector_type;          //!< alias

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                inline virtual       ~points() throw() {} //!< destructor
                inline points &       operator<<( const point &p ) { P.push_back(p); this->set_computed(false); return *this;}     //!< appending a point
                inline points &       add(const real x)                             { const POINT p(x);     return (*this) << p; } //!< append x
                inline points &       add(const real x, const real y)               { const POINT p(x,y);   return (*this) << p; } //!< append x,y
                inline points &       add(const real x, const real y, const real z) { const POINT p(x,y,z); return (*this) << p; } //!< append x,y,z

                inline virtual size_t size() const throw() { return P.size(); }    //!< #points

                //! const access
                inline const point &  operator[](const size_t index) const throw()
                { assert(index>0); assert(index<=P.size()); return P[index]; }

                //! address for low-level operations
                virtual const void *addr(const size_t index) const throw()
                { assert(index>0); assert(index<=P.size()); return  &P[index]; }


                //! t in [0:1] P1->PN
                inline point operator()(const real t) const
                {
                    const size_t n = this->size();
                    switch(n)
                    {
                        case 0: return zp;
                        case 1: return P[1];
                        default:break;
                    }
                    assert(n>=2);
                    if(t<=0)
                    {
                        return P[1];
                    }
                    else if(t>=1)
                    {
                        return P[n];
                    }
                    else
                    {
                        static const real six(6);
                        const size_t nm1 = n-1;
                        const real   tt  = real(1) + t*nm1;
                        const size_t jlo = min_of<real>(nm1,floor_of(tt));
                        const real   B   = (tt-jlo);
                        const size_t jup = jlo+1;
                        const real   A   = 1-B;
                        return A*P[jlo]+B*P[jup] + ((A*A*A-A) * Q[jlo] + (B*B*B-B) * Q[jup])/six;
                    }
                }

                //! constructor
                inline explicit points()  throw() :
                interface_for<real>(dim), P(), Q(), zp(0)
                {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(points);
                vector_type  P;   //!< points
                vector_type  Q;   //!< second derivatives
                friend class spline<POINT>;

            public:
                const point  zp;  //!< zero point
            };

            //! spline base class
            template <typename POINT>
            class spline
            {
            public:
                //______________________________________________________________
                //
                // definitions
                //______________________________________________________________
                typedef points<POINT>              points_type; //!< alias
                typedef typename points_type::real real;        //!< alias

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const style boundaries; //!< run-time style

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                //! destructor
                inline virtual ~spline() throw() {}

                //! 'time' to 'index' to save coordinates
                virtual real t2i( const real t, const size_t n) const throw() = 0;

                //______________________________________________________________
                //
                // non virtual interface
                //______________________________________________________________
                //! compute coefficients
                inline void compute( points_type &source )
                {
                    source.set_computed(false);
                    const array<POINT> &P = source.P;
                    source.Q.make(P.size(),source.zp);
                    array<POINT>       &Q = source.Q;
                    __compute(Q,P);
                    source.set_computed(true);
                }

                //! compute a point from data
                inline POINT compute( const real t, const points_type &source) const
                {
                    assert(source.computed);
                    return __compute(t,source.P,source.Q);
                }

            protected:
                //! constructor
                inline explicit spline(const style s) throw() : boundaries(s) {}
                //! compute coefficients
                virtual void  __compute( array<POINT> &Q, const array<POINT> &P ) = 0;
                //! interpolation
                virtual POINT __compute( const real t, const array<POINT> &P, const array<POINT> &Q ) const = 0;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(spline);
            };

            //! standard spline
            template <typename POINT>
            class standard_spline : public spline<POINT>
            {
            public:
                //______________________________________________________________
                //
                // definitions
                //______________________________________________________________
                typedef typename spline<POINT>::real real;                       //!< alias
                static const size_t                  dim = info_for<POINT>::dim; //!< static dimension

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                bool  lower_natural; //!< if lower boundary is natural, default=true
                bool  upper_natural; //!< if lower boundary is natural, default=true
                POINT lower_tangent; //!< lower tangent for non natural lower boundary
                POINT upper_tangent; //!< upper tangent for non natural upper boundary

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                //! destructor
                inline virtual ~standard_spline() throw() {}
                
                //! map t [0:1] to [1:N]
                inline virtual real t2i( const real t, const size_t n) const throw()
                {
                    static const real one(1);
                    if(n<=1)
                    {
                        return one;
                    }
                    else
                    {
                        const size_t nm1 = n-1;
                        return clamp<real>(one,one + t * nm1,n);
                    }
                }

                //______________________________________________________________
                //
                // non virtual interface
                //______________________________________________________________
                //! setup
                inline explicit standard_spline() throw() :
                spline<POINT>(standard),
                lower_natural(true),
                upper_natural(true),
                lower_tangent(0),
                upper_tangent(0)
                {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(standard_spline);
                virtual void __compute( array<POINT> &Q, const array<POINT> &P )
                {
                    assert(Q.size()==P.size());
                    const size_t n = P.size();
                    if(n<=1)
                    {
                        return;
                    }
                    else
                    {
                        static const real half(0.5);
                        static const real six(6);
                        static const real three(3);
                        static const real one(1);
                        static const real four(4);

                        tridiag<real> t(n);
                        arrays<real>  arr(2,n);
                        array<real>  &r = arr[0];
                        array<real>  &u = arr[1];

                        //______________________________________________________
                        //
                        // compute the matrix
                        //______________________________________________________
                        t.b[1] = t.b[n] = one;
                        for(size_t i=2;i<n;++i)
                        {
                            t.a[i] = t.c[i] = one;
                            t.b[i] = four;
                        }
                        if(!lower_natural)
                        {
                            t.c[1] = half;
                        }
                        if(!upper_natural)
                        {
                            t.a[n] = half;
                        }

                        //______________________________________________________
                        //
                        // compute dimension wise
                        //______________________________________________________
                        for(size_t d=0;d<dim;++d)
                        {
                            r[1] = r[n] =0;
                            for(size_t i=2;i<n;++i)
                            {
                                const real  pm = * ( (const real *)&P[i-1] + d);
                                const real  p0 = * ( (const real *)&P[i]   + d);
                                const real  pp = * ( (const real *)&P[i+1] + d);
                                r[i] = six * (pm+pp-(p0+p0));
                            }
                            if(!lower_natural)
                            {
                                const real lt = *( (const real *)&lower_tangent + d);
                                const real p1 = * ( (const real *)&P[1]+d);
                                const real p2 = * ( (const real *)&P[2]+d);
                                r[1] = three*( (p2-p1) - lt );
                            }

                            if(!lower_natural)
                            {
                                const real ut   =  *( (const real *)&upper_tangent + d);
                                const real pNm1 = * ( (const real *)&P[n-1]+d);
                                const real pN   = * ( (const real *)&P[n]+d);
                                r[n] = three*( ut - (pN-pNm1) );
                            }

                            t.solve(u,r);
                            for(size_t i=n;i>0;--i)
                            {
                                POINT &q = Q[i];
                                *((real *)&q + d) = u[i];
                            }
                        }
                    }
                }

                virtual POINT __compute( const real t, const array<POINT> &P, const array<POINT> &Q ) const
                {
                    assert(P.size()==Q.size());
                    const size_t        n = P.size();
                    switch(n)
                    {
                        case 0: return POINT(0);
                        case 1: return P[1];
                        default:
                            break;
                    }
                    assert(n>=2);
                    static const real zero(0);
                    static const real one(1);
                    if( t<=zero )
                    {
                        return P[1];
                    }
                    else if(t>=one)
                    {
                        return P[n];
                    }
                    else
                    {
                        static const real six(6);
                        const size_t nm1 = n-1;
                        const real   tt  = real(1) + t*nm1;
                        const size_t jlo = min_of<real>(nm1,floor_of(tt));
                        const real   B   = (tt-jlo);
                        const size_t jup = jlo+1;
                        const real   A   = 1-B;
                        return A*P[jlo]+B*P[jup] + ((A*A*A-A) * Q[jlo] + (B*B*B-B) * Q[jup])/six;
                    }
                }
            };

        };

        //! using float
        template <> struct curve:: info_for<float>
        {
            static const size_t dim = 1; //!< dim
            typedef float       real;    //!< real
        };

        //! using point2d<float>
        template <> struct curve:: info_for< point2d<float> >
        {
            static const size_t dim = 2; //!< dim
            typedef float       real;    //!< real
        };

        //! using complex<float>
        template <> struct curve:: info_for< complex<float> >
        {
            static const size_t dim = 2; //!< dim
            typedef float       real;    //!< real
        };

        //! using point2d<float>
        template <> struct curve:: info_for< point3d<float> >
        {
            static const size_t dim = 3;//!< dim
            typedef float       real;   //!< real
        };

        //! using double
        template <> struct curve:: info_for<double>
        {
            static const size_t dim = 1;//!< dim
            typedef double      real;   //!< real
        };

        //! using point2d<double>
        template <> struct curve:: info_for< point2d<double> >
        {
            static const size_t  dim = 2; //!< dim
            typedef double       real;    //!< real
        };

        //! using complex<double>
        template <> struct curve:: info_for< complex<double> >
        {
            static const size_t dim = 2; //!< dim
            typedef double      real;    //!< real
        };

        //! using point3d<double>
        template <> struct curve:: info_for< point3d<double> >
        {
            static const size_t dim = 3; //!< dim
            typedef double      real;    //!< real
        };
    }
}


#endif

