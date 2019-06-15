//! \file
#ifndef Y_MATH_ARC_INCLUDED
#define Y_MATH_ARC_INCLUDED 1

#include "y/sequence/vector.hpp"
#include "y/math/types.hpp"
#include "y/type/point3d.hpp"
#include "y/math/kernel/cyclic.hpp"
#include "y/ios/ostream.hpp"
#include "y/math/fcn/integrate.hpp"

namespace upsylon
{

    namespace math
    {

        struct arc
        {
            // utils
            template <typename POINT> struct info;

            template <typename T> static void save_real(ios::ostream &,const T &);

            template <typename POINT>
            static ios::ostream & save_point(ios::ostream &fp, const POINT &p )
            {
                typedef typename info<POINT>::real_type real_type;
                const real_type *r = (const real_type *)&p;
                save_real(fp,r[0]);
                for(size_t i=1;i< info<POINT>::dimension;++i)
                {
                    save_real(fp << ' ',r[i]);
                }
                return fp;
            }

            //! utility class
            template <typename POINT>
            class points : public vector<POINT>
            {
            public:
                typedef POINT                         point_type;
                typedef info<POINT>                   info_type;
                typedef typename info_type::real_type real_type;

                points & add(const real_type x)                                       { const POINT p(x);     this->push_back(p); return *this; }
                points & add(const real_type x, const real_type y)                    { const POINT p(x,y);   this->push_back(p); return *this; }
                points & add(const real_type x, const real_type y, const real_type z) { const POINT p(x,y,z); this->push_back(p); return *this; }

                inline explicit points() throw() : vector<POINT>() {}
                inline virtual ~points() throw() {}
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(points);
            };

            template <typename POINT>
            class spline : public vector<POINT>
            {
            public:
                typedef typename info<POINT>::real_type real_type;

                inline virtual ~spline() throw() {}

                //! interface to compute coefficients
                inline void compute( const array<POINT> &P )
                {
                    const size_t n = P.size();
                    this->make(n,origin);
                    if(n>1)
                    {
                        try
                        {
                            __compute( *this, P );
                        }
                        catch(...)
                        {
                            this->free();
                            throw;
                        }
                    }
                }

                //! interface for interpolation
                inline void compute(POINT *M, POINT *dMdt, POINT *d2Mdt2, const real_type t, const array<POINT> &P) const throw()
                {
                    assert(this->size()==P.size());
                    switch( this->size() )
                    {
                        case 0: if(M) *M = origin; if(dMdt) *dMdt = origin; if(d2Mdt2) *d2Mdt2 = origin; break;
                        case 1: if(M) *M = P[1];   if(dMdt) *dMdt = origin; if(d2Mdt2) *d2Mdt2 = origin; break;
                        default: __compute(M,dMdt,d2Mdt2,t,P,*this);
                    }
                }

                //! position helper
                inline POINT position(const real_type t, const array<POINT> &P, POINT *dMdt=0, POINT *d2Mdt2=0) const throw()
                {
                    POINT p = origin;
                    compute(&p,dMdt,d2Mdt2,t,P);
                    return p;
                }

                //! tangent helper
                inline POINT tangent(const real_type t, const array<POINT> &P, POINT *d2Mdt2=0) const throw()
                {
                    POINT s = origin;
                    compute(0,&s,d2Mdt2,t,P);
                    return s;
                }

                //! |tangent|
                inline real_type speed(const real_type t, const array<POINT> &P ) const throw()
                {
                    const POINT     s  = tangent(t,P);
                    const real_type s2 = info<POINT>::norm2(s);
                    return sqrt_of(s2);
                }

                //! extent
                inline real_type extent(const real_type t0, const real_type t1,const array<POINT> &P) const
                {
                    const d_extent f = { this,&P };
                    return integrate::compute<real_type,const d_extent>(f,t0,t1,1e-4);
                }


            protected:
                inline explicit spline() throw() : vector<POINT>(), origin(0) {}

                //! compute coefficients for more than one point
                virtual void __compute( array<POINT> &Q, const array<POINT> &P ) = 0;

                //! interpolation for more than one point
                virtual void __compute( POINT *M, POINT *dMdt, POINT *d2Mdt2, const real_type t, const array<POINT> &P, const array<POINT> &Q ) const throw() = 0;

                //! helper
                inline void __get(POINT *M, POINT *dMdt, POINT *d2Mdt2,
                                  const real_type   A,   const real_type   B,
                                  const size_t jlo, const size_t jup,
                                  const array<POINT> &P, const array<POINT> &Q) const throw()
                {
                    static const real_type one(1);
                    static const real_type one_sixth = one/6;

                    assert(jlo>=1);
                    assert(jlo<=P.size());
                    assert(jup>=1);
                    assert(jup<=P.size());
                    const real_type A2  = A*A;
                    const real_type B2  = B*B;
                    const POINT     PA = P[jlo];
                    const POINT     PB = P[jup];
                    const POINT     QA = Q[jlo];
                    const POINT     QB = Q[jup];

                    if(M)      *M      = A*PA + B*PB + (A*(A2-one) * QA + B*(B2-one) * QB)*one_sixth;
                    if(dMdt)   *dMdt   = ( (PB-PA) + one_sixth * ( (3*B2-one) * QB - (3*A2-one)*QA) );
                    if(d2Mdt2) *d2Mdt2 = (A*QA+B*QB);
                }

                struct d_extent
                {
                    const spline       *self;
                    const array<POINT> *data;
                    inline real_type operator()( const real_type t ) const throw()
                    {
                        return self->speed(t,*data);
                    }
                };

            private:
                Y_DISABLE_COPY_AND_ASSIGN(spline);

            public:
                const POINT origin;

            };

            template <typename POINT>
            class standard_spline : public spline<POINT>
            {
            public:
                typedef typename     info<POINT>::real_type real_type;
                static  const size_t                        dimension = info<POINT>::dimension;

                bool  lower_natural;
                bool  upper_natural;
                POINT lower_tangent;
                POINT upper_tangent;
                POINT S1;
                POINT SN;
                POINT Q1;
                POINT QN;

                inline explicit standard_spline(const bool ln=true,const bool un=true) throw() :
                spline<POINT>(),
                lower_natural(ln),
                upper_natural(un),
                lower_tangent(0),
                upper_tangent(0),
                S1(0),
                SN(0),
                Q1(0),
                QN(0)
                {}


            private:
                Y_DISABLE_COPY_AND_ASSIGN(standard_spline);
                virtual void __compute(  array<POINT> &Q, const array<POINT> &P )
                {
                    static const real_type half(0.5);
                    static const real_type one(1);
                    static const real_type three(3);
                    static const real_type four(4);
                    static const real_type six(6);;

                    assert( P.size() > 1 );
                    assert( Q.size() == P.size() );

                    const size_t       n    = P.size();
                    const size_t       nm1 = n-1;
                    tridiag<real_type> t(n,2);
                    array<real_type>  &r   = t[0];
                    array<real_type>  &u   = t[1];

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
                    for(size_t d=0;d<dimension;++d)
                    {
                        r[1] = r[n] =0;
                        for(size_t i=2;i<n;++i)
                        {
                            const real_type  pm = * ( (const real_type *)&P[i-1] + d);
                            const real_type  p0 = * ( (const real_type *)&P[i]   + d);
                            const real_type  pp = * ( (const real_type *)&P[i+1] + d);
                            r[i] = six * (pm+pp-(p0+p0));
                        }

                        if(!lower_natural)
                        {
                            const real_type lt = *( (const real_type *)&lower_tangent + d);
                            const real_type p1 = *( (const real_type *)&P[1]+d);
                            const real_type p2 = *( (const real_type *)&P[2]+d);
                            r[1] = three*( (p2-p1) - lt );
                        }

                        if(!upper_natural)
                        {
                            const real_type ut   =  *( (const real_type *)&upper_tangent + d);
                            const real_type pNm1 = * ( (const real_type *)&P[nm1]+d);
                            const real_type pN   = * ( (const real_type *)&P[n]+d);
                            r[n] = three*( ut - (pN-pNm1) );
                        }

                        t.solve(u,r);
                        for(size_t i=n;i>0;--i)
                        {
                            POINT &q = Q[i];
                            *((real_type *)&q + d) = u[i];
                        }
                    }

                    //______________________________________________________
                    //
                    // compute boundary tangents/secondary
                    //______________________________________________________
                    static const real_type   one_third = one/3;
                    static const real_type   one_sixth = one/6;
                    if(lower_natural)
                    {
                        S1 = (P[2] - P[1]) - one_third * Q[1] - one_sixth * Q[2];
                    }
                    else
                    {
                        S1 = lower_tangent;
                    }

                    if(upper_natural)
                    {
                        SN = (P[n]-P[nm1]) + one_third * Q[n] + one_sixth * Q[nm1];
                    }
                    else
                    {
                        SN = upper_tangent;
                    }

                    Q1 = Q[1];
                    QN = Q[n];

                }

                virtual void __compute( POINT *M, POINT *dMdt, POINT *d2Mdt2, const real_type t, const array<POINT> &P, const array<POINT> &Q ) const throw()
                {
                    static const real_type one(1);

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
                        const size_t    jlo = clamp<size_t>(1,floor_of(t),n-1);
                        const real_type B   = (t-jlo);
                        const size_t    jup = jlo+1;
                        const real_type A   = one-B;

                        this->__get(M, dMdt, d2Mdt2, A, B, jlo, jup, P, Q);
                    }
                }


            };

            template <typename POINT>
            class periodic_spline : public spline<POINT>
            {
            public:
                typedef typename     info<POINT>::real_type real_type;
                static  const size_t                        dimension = info<POINT>::dimension;

                inline explicit periodic_spline() throw() :
                spline<POINT>()
                {}


            private:
                Y_DISABLE_COPY_AND_ASSIGN(periodic_spline);

                inline real_type get_r(const size_t        d,
                                       const array<POINT> &P,
                                       const size_t im, const size_t i0, const size_t ip) const throw()
                {
                    static const real_type six(6);
                    const real_type pm = *( (const real_type *) &P[im] + d);
                    const real_type p0 = *( (const real_type *) &P[i0] + d);
                    const real_type pp = *( (const real_type *) &P[ip] + d);
                    return six * (pm - (p0+p0) + pp );
                }

                virtual void __compute(  array<POINT> &Q, const array<POINT> &P )
                {

                    assert( P.size() > 1 );
                    assert( Q.size() == P.size() );

                    const size_t n = P.size();
                    if(n>=3)
                    {
                        cyclic<real_type> cyc(n,2);
                        array<real_type>  &u   = cyc[0];
                        array<real_type>  &r   = cyc[1];
                        const size_t       nm1 = n-1;

                        // prepare
                        cyc.set(1,4,1);

                        for(size_t d=0;d<dimension;++d)
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
                                *( (real_type *)&Q[i] + d) = u[i];
                            }
                        }
                    }
                }

                virtual void __compute( POINT *M, POINT *dMdt, POINT *d2Mdt2, const real_type t, const array<POINT> &P, const array<POINT> &Q ) const throw()
                {
                    static const real_type one(1);

                    // get the sample size
                    assert(P.size()>1);
                    assert(P.size()==Q.size());
                    const size_t n = P.size();

                    // map position
                    real_type tt = t;
                    while(tt>n) tt -= n;
                    while(tt<1) tt += n;

                    // get bracketing indices
                    const size_t    jlo = clamp<size_t>(1,floor_of(tt),n);
                    size_t          jup = jlo+1; if(jup>n) jup = 1;
                    const real_type B   = (tt-jlo);
                    const real_type A   = one-B;

                    // compute all
                    this->__get(M, dMdt, d2Mdt2, A, B, jlo, jup, P, Q);
                }

            };


        };

#define  Y_MATH_ARC_INFO(TYPE,REAL,CODE)                           \
template <> struct arc::info< TYPE > {                             \
typedef TYPE            type;                                      \
typedef REAL            real_type;                                 \
static const size_t     dimension = sizeof(type)/sizeof(real_type);\
static inline real_type norm2( const type &p ) { return (CODE); }  \
}

#define Y_MATH_ARC_NORM2_REAL p*p
#define Y_MATH_ARC_NORM2_PT2D (p.x*p.x)+(p.y*p.y)
#define Y_MATH_ARC_NORM2_CPLX (p.re*p.re)+(p.im*p.im)
#define Y_MATH_ARC_NORM2_PT3D (p.x*p.x)+(p.y*p.y)+(p.z*p.z)

#define Y_MATH_ARC_INFO_IMPL(REAL) \
Y_MATH_ARC_INFO(REAL,REAL,         Y_MATH_ARC_NORM2_REAL);\
Y_MATH_ARC_INFO(point2d<REAL>,REAL,Y_MATH_ARC_NORM2_PT2D);\
Y_MATH_ARC_INFO(complex<REAL>,REAL,Y_MATH_ARC_NORM2_CPLX);\
Y_MATH_ARC_INFO(point3d<REAL>,REAL,Y_MATH_ARC_NORM2_PT3D)

        Y_MATH_ARC_INFO_IMPL(float);
        Y_MATH_ARC_INFO_IMPL(double);

    }

}


#endif

