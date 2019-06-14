//! \file

#ifndef Y_SIGNAL_CURVE_INCLUDED
#define Y_SIGNAL_CURVE_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/math/kernel/cyclic.hpp"
#include "y/sequence/vector.hpp"
#include "y/ios/ostream.hpp"
#include "y/math/fcn/integrate.hpp"

namespace upsylon
{
    namespace math
    {
        //! 1D/2D/3D... spline
        struct curve
        {
            //! template to extract info from types
            template <typename T> struct info_for;

            //! wrapper to get squared norm of a point
            template <typename POINT> static inline
            typename info_for<POINT>::real norm2( const POINT &P ) throw()
            {
                return info_for<POINT>::norm2(P);
            }

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
                ios::ostream &save_point( ios::ostream &fp, const void *p ) const;       //!< save a point
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
            template <typename POINT>
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
                typedef vector<point>            vector_type;          //!< alias

                
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
                    {
                        const array<POINT> &P = source.P;
                        const size_t n = P.size();
                        if(n>1)
                        {
                            source.Q.make(P.size(),source.zp);
                            array<POINT>       &Q = source.Q;
                            __compute(Q,P);
                        }
                    }
                    source.set_computed(true);
                }

                //! compute a point from data
                inline POINT compute( const real t, const points_type &source) const throw()
                {
                    assert(source.computed);
                    const array<POINT> &P = source.P;
                    switch( P.size() )
                    {
                        case 0: return source.zp;
                        case 1: return P[1];
                        default: break;
                    }
                    return __compute(t,P,source.Q);
                }

                //! compute tangent from data
                inline POINT tangent( const real t, const points_type &source, POINT *accel ) const throw()
                {
                    assert(source.computed);
                    const array<POINT> &P = source.P;
                    switch( P.size() )
                    {
                        case 0: if(accel) *accel = source.zp; return source.zp;
                        case 1: if(accel) *accel = source.zp; return source.zp;
                        default: break;
                    }
                    return __tangent(t,P,source.Q,NULL);
                }

                //! compute a point from data and derivative at once
                inline POINT compute( const real t, const points_type &source, POINT &S) const throw()
                {
                    assert(source.computed);
                    const array<POINT> &P = source.P;
                    switch( P.size() )
                    {
                        case 0: S=source.zp; return source.zp;
                        case 1: S=source.zp; return P[1];
                        default: break;
                    }
                    return __compute(t,P,source.Q,S);
                }

                //! |tangent|
                inline real speed( const real t, const points_type &source ) const throw()
                {
                    const POINT p = tangent(t,source,NULL);
                    return sqrt_of( norm2(p) );
                }

                //! arc length
                inline real arc_length(const real t0, const real t1, const points_type &source ) const
                {
                    d_arc F = { this, &source };
                    return integrate::compute(F,t0,t1,real(1e-4));
                }

                
            protected:
                //! constructor
                inline explicit spline(const style s) throw() : boundaries(s) {}

                //! compute coefficients for more than 1 point
                virtual void  __compute( array<POINT> &Q, const array<POINT> &P ) = 0;

                //! interpolation for more than one point
                virtual POINT __compute( const real t, const array<POINT> &P, const array<POINT> &Q ) const throw() = 0;


                //! tangent for more than one point
                virtual POINT __tangent( const real t, const array<POINT> &P, const array<POINT> &Q, POINT *accel) const throw() = 0;

                //! interpolation and tangent
                virtual POINT __compute(const real t, const array<POINT> &P, const array<POINT> &Q, POINT &S) const throw() = 0;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(spline);
                struct d_arc
                {
                    const spline      *self;
                    const points_type *data;
                    inline real operator()(const real t) const throw() { return self->speed(t,*data); }
                };

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
                upper_tangent(0),
                S0(0),
                SN(0)
                {}



            private:
                Y_DISABLE_COPY_AND_ASSIGN(standard_spline);
                POINT S0;
                POINT SN;

                virtual void __compute( array<POINT> &Q, const array<POINT> &P )
                {
                    static const real half(0.5);
                    static const real six(6);
                    static const real three(3);
                    static const real one(1);
                    static const real four(4);
                    assert(P.size()>1);
                    assert(Q.size()==P.size());
                    const size_t n = P.size();
                    
                    tridiag<real> t(n,2);
                    array<real>  &r = t[0];
                    array<real>  &u = t[1];

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
                            const real p1 = *( (const real *)&P[1]+d);
                            const real p2 = *( (const real *)&P[2]+d);
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
                    //______________________________________________________
                    //
                    // compute boundary tangents
                    //______________________________________________________
                    static const real   one_third = one/3;
                    static const real   one_sixth = one/6;
                    const        size_t nm1       = n-1;
                    S0 = (P[2] - P[1]) - one_third * Q[1] - one_sixth * Q[2];
                    SN = (P[n]-P[nm1]) + one_third * Q[n] + one_sixth * Q[nm1];
                }

                virtual inline POINT __compute( const real t, const array<POINT> &P, const array<POINT> &Q ) const throw()
                {
                    static const real zero(0);
                    static const real one(1);
                    static const real one_sixth = one/6;

                    assert(P.size()>1);
                    assert(P.size()==Q.size());

                    if( t<=zero )
                    {
                        return P[1];
                    }
                    else if(t>=one)
                    {
                        return P[P.size()];
                    }
                    else
                    {
                        const size_t n   = P.size();
                        const size_t nm1 = n-1;
                        const real   tt  = real(1) + t*nm1;
                        const size_t jlo = min_of<real>(nm1,floor_of(tt));
                        const real   B   = (tt-jlo);
                        const size_t jup = jlo+1;
                        const real   A   = one-B;
                        const real   A2  = A*A;
                        const real   B2  = B*B;
                        return A*P[jlo]+B*P[jup] + (A*(A2-one) * Q[jlo] + B*(B2-one) * Q[jup])*one_sixth;
                    }
                }

                inline virtual POINT __tangent( const real t, const array<POINT> &P, const array<POINT> &Q, POINT *accel ) const throw()
                {
                    static const real zero(0);
                    static const real one(1);
                    static const real one_sixth = one/6;

                    assert(P.size()>1);
                    assert(P.size()==Q.size());

                    if( t <= zero )
                    {
                        if(accel) *accel = Q[1];
                        return S0;
                    }
                    else if( t>= one )
                    {
                        if(accel) *accel = Q[Q.size()];
                        return SN;
                    }
                    else
                    {
                        const size_t n   = P.size();
                        const size_t nm1 = n-1;
                        const real   tt  = real(1) + t*nm1;
                        const size_t jlo = min_of<real>(nm1,floor_of(tt));
                        const real   B   = (tt-jlo);
                        const size_t jup = jlo+1;
                        const real   A   = one-B;
                        const real   B2  = B*B;
                        const real   A2  = A*A;
                        if(accel) *accel = A*Q[jlo] + B*Q[jup];
                        return P[jup]-P[jlo] + one_sixth * ( (3*B2-one) * Q[jup] - (3*A2-one)*Q[jlo]);
                    }
                }

                inline virtual POINT __compute(const real t, const array<POINT> &P, const array<POINT> &Q, POINT &S) const throw()
                {
                    static const real zero(0);
                    static const real one(1);
                    static const real one_sixth = one/6;

                    assert(P.size()>1);
                    assert(P.size()==Q.size());
                    if( t <= zero )
                    {
                        S = S0;
                        return P[1];
                    }
                    else if( t >= one )
                    {
                        S = SN;
                        return P[ P.size() ];
                    }
                    else
                    {
                        const size_t n   = P.size();
                        const size_t nm1 = n-1;
                        const real   tt  = real(1) + t*nm1;
                        const size_t jlo = min_of<real>(nm1,floor_of(tt));
                        const real   B   = (tt-jlo);
                        const size_t jup = jlo+1;
                        const real   A   = one-B;
                        const real   A2  = A*A;
                        const real   B2  = B*B;

                        S = P[jup]-P[jlo] + one_sixth * ( (3*B2-one) * Q[jup] - (3*A2-one)*Q[jlo]);
                        return A*P[jlo]+B*P[jup] + (A*(A2-one) * Q[jlo] + B*(B2-one) * Q[jup])*one_sixth;
                    }
                }

            };


            //! standard spline
            template <typename POINT>
            class periodic_spline : public spline<POINT>
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
                // virtual interface
                //______________________________________________________________
                //! destructor
                inline virtual ~periodic_spline() throw() {}

                //! map t [0:1] to [1:N+1]
                inline virtual real t2i( const real t, const size_t n) const throw()
                {
                    static const real one(1);
                    if(n<=1)
                    {
                        return one;
                    }
                    else
                    {
                        real tt = one + t * n;
                        return tt;
                    }
                }

                //______________________________________________________________
                //
                // non virtual interface
                //______________________________________________________________
                //! setup
                inline explicit periodic_spline() throw() : spline<POINT>(periodic) {}




            private:
                Y_DISABLE_COPY_AND_ASSIGN(periodic_spline);
                inline real get_r(const size_t d,
                                  const array<POINT> &P,
                                  const size_t im, const size_t i0, const size_t ip) const throw()
                {
                    static const real six(6);
                    const real pm = *( (real *) &P[im] + d);
                    const real p0 = *( (real *) &P[i0] + d);
                    const real pp = *( (real *) &P[ip] + d);
                    return six * (pm - (p0+p0) + pp );
                }

                virtual void __compute( array<POINT> &Q, const array<POINT> &P )
                {
                    assert(P.size()>1);
                    assert(Q.size()==P.size());
                    const size_t n = P.size();
                    if(n>=3)
                    {
                        cyclic<real> cyc(n,2);
                        array<real>  &u   = cyc[0];
                        array<real>  &r   = cyc[1];
                        const size_t  nm1 = n-1;

                        // prepare
                        cyc.set(1,4,1);

                        for(size_t d=0;d<dim;++d)
                        {
                            // fill rhs
                            r[1] = get_r(d,P,n,1,2);
                            for(size_t im=1,i0=2,ip=3;;)
                            {
                                r[i0] = get_r(d,P,im,i0,ip);
                                im  = i0;
                                i0  = ip;
                                if(i0>=n) break;
                                ++ip;
                            }
                            r[n] = get_r(d,P,nm1,n,1);

                            // solve
                            cyc.solve(u,r);

                            // dispatch
                            for(size_t i=n;i>0;--i)
                            {
                                *( (real *)&Q[i] + d) = u[i];
                            }
                        }
                    }
                }

                static inline real __map( const real t, const size_t n) throw()
                {
                    static const real one(1);
                    real tt = one + t * n;
                    const size_t np1 = n+1;
                    while(tt<one) tt += n;
                    while(tt>np1) tt -= n;
                    return tt;
                }

                inline virtual POINT __compute( const real t, const array<POINT> &P, const array<POINT> &Q ) const throw()
                {
                    static const real one(1);
                    static const real six(6);
                    
                    // get the sample size
                    assert(P.size()>1);
                    assert(P.size()==Q.size());
                    const size_t n = P.size();

                    // map position
                    const real tt = __map(t,n);

                    // get bracketing indices
                    const size_t  jlo = clamp<size_t>(1,floor_of(tt),n);
                    size_t        jup = jlo+1; if(jup>n) jup = 1;
                    const real    B   = (tt-jlo);
                    const real    A   = one-B;

                    return A*P[jlo]+B*P[jup] + ((A*A*A-A) * Q[jlo] + (B*B*B-B) * Q[jup])/six;
                }

                inline virtual POINT __tangent( const real t, const array<POINT> &P, const array<POINT> &Q, POINT *accel ) const throw()
                {
                    static const real one(1);
                    static const real one_sixth = one/6;
                    

                    // map position
                    const size_t n = P.size();
                    const real tt = __map(t,n);
                    
                    // get bracketing indices
                    const size_t  jlo = clamp<size_t>(1,floor_of(tt),n);
                    size_t        jup = jlo+1; if(jup>n) jup = 1;
                    const real    B   = (tt-jlo);
                    const real    A   = one-B;
                    if(accel) *accel = A*Q[jlo] + B*Q[jup];
                    return P[jup]-P[jlo] + one_sixth * ( (3*B*B-1) * Q[jup] - (3*A*A-1)*Q[jlo]);
                }

                inline virtual POINT __compute(const real t, const array<POINT> &P, const array<POINT> &Q, POINT &S) const throw()
                {
                    static const real one(1);
                    static const real one_sixth = one/6;

                    assert(P.size()>1);
                    assert(P.size()==Q.size());

                    // map position
                    const size_t n    = P.size();
                    const real   tt   = __map(t,n);
                    // get indices
                    const size_t  jlo = clamp<size_t>(1,floor_of(tt),n);
                    size_t        jup = jlo+1; if(jup>n) jup = 1;
                    const real    B   = (tt-jlo);
                    const real    A   = one-B;
                    const real    A2  = A*A;
                    const real    B2  = B*B;

                    S = P[jup]-P[jlo] + one_sixth * ( (3*B2-one) * Q[jup] - (3*A2-one)*Q[jlo]);
                    return A*P[jlo]+B*P[jup] + (A*(A2-one) * Q[jlo] + B*(B2-one) * Q[jup])*one_sixth;
                }
            };


        };

        //! using float
        template <> struct curve:: info_for<float>
        {
            static const size_t dim = 1; //!< dim
            typedef float       real;    //!< real
            //! |p|^2
            static inline real norm2( const real p ) throw() { return p*p; }
        };

        //! using point2d<float>
        template <> struct curve:: info_for< point2d<float> >
        {
            static const size_t dim = 2; //!< dim
            typedef float       real;    //!< real
            //! |p|^2
            static inline real  norm2( const point2d<real> &p ) throw() { return p.x*p.x+p.y*p.y; }
        };

        //! using complex<float>
        template <> struct curve:: info_for< complex<float> >
        {
            static const size_t dim = 2; //!< dim
            typedef float       real;    //!< real
            //! |p|^2
            static inline real  norm2( const complex<real> &p ) throw() { return p.re*p.re+p.im*p.im; }
        };

        //! using point2d<float>
        template <> struct curve:: info_for< point3d<float> >
        {
            static const size_t dim = 3;//!< dim
            typedef float       real;   //!< real
            //! |p|^2
            static inline real  norm2( const point3d<real> &p ) throw() { return p.x*p.x+p.y*p.y+p.z*p.z; }
        };

        //! using double
        template <> struct curve:: info_for<double>
        {
            static const size_t dim = 1;//!< dim
            typedef double      real;   //!< real
            //! |p|^2
            static inline real norm2( const real p ) throw() { return p*p; }
        };

        //! using point2d<double>
        template <> struct curve:: info_for< point2d<double> >
        {
            static const size_t  dim = 2; //!< dim
            typedef double       real;    //!< real
            //! |p|^2
            static inline real   norm2( const point2d<real> &p ) throw() { return p.x*p.x+p.y*p.y; }
        };

        //! using complex<double>
        template <> struct curve:: info_for< complex<double> >
        {
            static const size_t dim = 2; //!< dim
            typedef double      real;    //!< real
            //! |p|^2
            static inline real  norm2( const complex<real> &p ) throw() { return p.re*p.re+p.im*p.im; }

        };

        //! using point3d<double>
        template <> struct curve:: info_for< point3d<double> >
        {
            static const size_t dim = 3; //!< dim
            typedef double      real;    //!< real
            //! |p|^2
            static inline real  norm2( const point3d<real> &p ) throw() { return p.x*p.x+p.y*p.y+p.z*p.z; }

        };
    }
}


#endif

