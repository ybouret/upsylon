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
                typedef points<POINT>              points_type;            //!< alias
                typedef typename points_type::real real;                   //!< alias
                static  const size_t               dim = points_type::dim; //!< alias
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const style boundaries; //!< run-time style
                const POINT zp;         //!< zerop point

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                //! destructor
                inline virtual ~spline() throw() {}
                

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

                //! all cases computation
                inline void compute(POINT *M, POINT *dMdt, POINT *d2Mdt2, const real t, const points_type &source) const throw()
                {
                    assert(source.computed);
                    const array<POINT> &P = source.P;
                    switch(P.size())
                    {
                        case 0:  if(M) *M=zp;   if(dMdt) *dMdt=zp; if(d2Mdt2) *d2Mdt2=zp; break;
                        case 1:  if(M) *M=P[1]; if(dMdt) *dMdt=zp; if(d2Mdt2) *d2Mdt2=zp; break;
                        default: __compute(M,dMdt,d2Mdt2, t,P,source.Q);
                    }
                }

                //! helper to get position
                inline POINT compute(const real t, const points_type &source, POINT *dMdt=0, POINT *d2Mdt2=0) const throw()
                {
                    POINT M = zp;
                    compute(&M,dMdt,d2Mdt2,t,source);
                    return M;
                }

                //! helper to get tangent
                inline POINT tangent(const real t, const points_type &source, POINT *d2Mdt2=0) const throw()
                {
                    POINT dMdt = zp;
                    compute(NULL,&dMdt,d2Mdt2,t,source);
                    return dMdt;
                }

                //! |tangent|
                inline real speed( const real t, const points_type &source ) const throw()
                {
                    POINT dMdt = zp;
                    compute(NULL,&dMdt,NULL,t,source);
                    return sqrt_of( curve::norm2(dMdt) );
                }

                //! compute curvature
                inline real curvature( const real t, const points_type &source ) const throw()
                {
                    return (source.P.size() > 1) ? __curvature<dim>(t,source) : 0;
                }

                //! arc length
#if 0
                inline real arc_length(const real t0, const real t1, const points_type &source ) const
                {
                    d_arc F = { this, &source };
                    //return integrate::compute<real,d_arc>(F,t0,t1,numeric<real>::sqrt_ftol);
                    return 0;
                }
#endif
                
            protected:
                //! constructor
                inline explicit spline(const style s) throw() : boundaries(s), zp(0) {}

                //! compute coefficients for more than 1 point
                virtual void  __compute( array<POINT> &Q, const array<POINT> &P ) = 0;

                //! interpolation for more than one point
                virtual void __compute( POINT *M, POINT *dMdt, POINT *d2Mdt2, const real t, const array<POINT> &P, const array<POINT> &Q ) const throw() = 0;

                inline void __get(POINT *M, POINT *dMdt, POINT *d2Mdt2,
                                  const real   A,   const real   B,
                                  const size_t jlo, const size_t jup,
                                  const array<POINT> &P, const array<POINT> &Q) const throw()
                {
                    static const real one(1);
                    static const real one_sixth = one/6;

                    assert(jlo>=1);
                    assert(jlo<=P.size());
                    assert(jup>=1);
                    assert(jup<=P.size());
                    const real   A2  = A*A;
                    const real   B2  = B*B;
                    const POINT  PA = P[jlo];
                    const POINT  PB = P[jup];
                    const POINT  QA = Q[jlo];
                    const POINT  QB = Q[jup];

                    if(M)      *M      = A*PA + B*PB + (A*(A2-one) * QA + B*(B2-one) * QB)*one_sixth;
                    if(dMdt)   *dMdt   = ( (PB-PA) + one_sixth * ( (3*B2-one) * QB - (3*A2-one)*QA) );
                    if(d2Mdt2) *d2Mdt2 = (A*QA+B*QB);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(spline);
                struct d_arc
                {
                    const spline      *self;
                    const points_type *data;
                    inline real operator()(const real t) const throw() { return self->speed(t,*data); }
                };

                template <const size_t DIM>
                real __curvature(const real, const points_type &) const throw();

                template <>
                real __curvature<1>(const real t, const points_type &source) const throw()
                {
                    assert(source.P.size()>1);
                    real v=0,a=0;
                    __compute(NULL,&v,&a,t,source.P,source.Q);
                    const real den2 = 1 + v*v;
                    return a/sqrt_of(den2*den2*den2);
                }

                template <>
                real __curvature<2>(const real t, const points_type &source) const throw()
                {
                    assert(source.P.size()>1);
                    POINT v,a;
                    __compute(NULL,&v,&a,t,source.P,source.Q);
                    const real *pv = (const real *) &v;
                    const real *pa = (const real *) &a;
                    const real  vx = pv[0], vy=pv[1];
                    const real  ax = pa[0], ay=pa[1];
                    const real  v2 = vx*vx + vy*vy;
                    if(v2<=0)
                    {
                        return 0;
                    }
                    else
                    {
                        return (vx*ay-vy*ax)/sqrt_of( v2*v2*v2 );
                    }
                }

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
                S1(0),
                SN(0),
                Q1(0),
                QN(0)
                {}



            private:
                Y_DISABLE_COPY_AND_ASSIGN(standard_spline);
                size_t nm1;
                size_t nm1sq;
                POINT S1;
                POINT SN;
                POINT Q1;
                POINT QN;

                virtual void __compute( array<POINT> &Q, const array<POINT> &P )
                {
                    static const real half(0.5);
                    static const real six(6);
                    static const real three(3);
                    static const real one(1);
                    static const real four(4);
                    assert(P.size()>1);
                    assert(Q.size()==P.size());
                    const size_t n   = P.size();
                    const size_t nm1 = n-1;

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
                    S1 = (P[2] - P[1]) - one_third * Q[1] - one_sixth * Q[2];
                    SN = (P[n]-P[nm1]) + one_third * Q[n] + one_sixth * Q[nm1];
                }


                virtual void __compute( POINT *M, POINT *dMdt, POINT *d2Mdt2, const real t, const array<POINT> &P, const array<POINT> &Q ) const throw()
                {
                    static const real one(1);

                    assert(P.size()>1);
                    assert(P.size()==Q.size());
                    const size_t n = P.size();
                    if( t<=1 )
                    {
                        if(M)      *M      = P[1];
                        if(dMdt)   *dMdt   = S1;
                        if(d2Mdt2) *d2Mdt2 = Q1;
                    }
                    else if(t>=n)
                    {
                        if(M)       *M      = P[n];
                        if(dMdt)    *dMdt   = SN;
                        if(d2Mdt2)  *d2Mdt2 = QN;
                    }
                    else
                    {
                        const size_t jlo = clamp<size_t>(1,floor_of(t),n-1);
                        const real   B   = (t-jlo);
                        const size_t jup = jlo+1;
                        const real   A   = one-B;

                        this->__get(M, dMdt, d2Mdt2, A, B, jlo, jup, P, Q);
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

                //______________________________________________________________
                //
                // non virtual interface
                //______________________________________________________________
                //! setup
                inline explicit periodic_spline() throw() : spline<POINT>(periodic) {}

                inline real area(const points<POINT> &source) const
                {
                    return __area<dim>(source);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(periodic_spline);
                struct d_area
                {
                    const periodic_spline *self;
                    const points<POINT>   *data;
                    inline real operator()(const real t) const throw()
                    {
                        POINT       p,v;
                        self->compute(&p,&v,0,t,*data);
                        const real *pp = (const real *)&p;
                        const real *pv = (const real *)&v;
                        const real x  = pp[0], y  = pp[1];
                        const real vx = pv[0], vy = pv[1];
                        return x*vy - y*vx;
                    }
                };

                template <const size_t> real __area(const points<POINT> &source) const;
                template <> real __area<2>(const points<POINT> &source) const
                {
                    d_area F = { this, &source };
                    //return fabs_of(integrate::compute<real,d_area>(F,0,1,numeric<real>::sqrt_ftol))/2;
                    return 0;
                }


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


                virtual void __compute( POINT *M, POINT *dMdt, POINT *d2Mdt2, const real t, const array<POINT> &P, const array<POINT> &Q ) const throw()
                {
                    static const real one(1);

                    // get the sample size
                    assert(P.size()>1);
                    assert(P.size()==Q.size());
                    const size_t n = P.size();

                    // map position
                    real tt = t;
                    while(tt>n) tt -= n;
                    while(tt<1) tt += n;

                    // get bracketing indices
                    const size_t  jlo = clamp<size_t>(1,floor_of(tt),n);
                    size_t        jup = jlo+1; if(jup>n) jup = 1;
                    const real    B   = (tt-jlo);
                    const real    A   = one-B;

                    // compute all
                    this->__get(M, dMdt, d2Mdt2, A, B, jlo, jup, P, Q);
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

