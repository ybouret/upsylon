//! \file

#ifndef Y_SIGNAL_CURVE_INCLUDED
#define Y_SIGNAL_CURVE_INCLUDED 1

#include "y/type/point3d.hpp"
#include "y/math/kernel/tridiag.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace math
    {
        struct curve
        {
            template <typename T> struct info_for;

            template <> struct info_for<float>
            {
                static const size_t dim = 1;
                typedef float       real;
            };

            template <> struct info_for< point2d<float> >
            {
                static const size_t dim = 2;
                typedef float       real;
            };

            template <> struct info_for< point3d<float> >
            {
                static const size_t dim = 3;
                typedef float       real;
            };

            template <> struct info_for<double>
            {
                static const size_t dim = 1;
                typedef double      real;
            };

            template <> struct info_for< point2d<double> >
            {
                static const size_t  dim = 2;
                typedef double       real;
            };

            template <> struct info_for< point3d<double> >
            {
                static const size_t dim = 3;
                typedef double      real;
            };

            class interface
            {
            public:
                virtual ~interface() throw();
                virtual size_t size() const throw() = 0;

            protected:
                explicit interface() throw();
                bool         ok;  //!< keep track

            private:
                Y_DISABLE_COPY_AND_ASSIGN(interface);
            };

            template <
            typename POINT,
            typename ALLOCATOR= memory::global
            >
            class points : public interface
            {
            public:
                typedef info_for<POINT>          info_type;
                static const size_t              dim = info_type::dim;
                typedef POINT                    point;
                typedef typename info_type::real real;
                typedef vector<point,ALLOCATOR>  vector_type;

                inline explicit points() throw() : P(), Q(), zp(0) {}
                inline virtual ~points() throw() {}


                inline void append( const point &p )         { P.push_back(p); ok = false; }
                inline points & operator<<( const point &p ) { append(p);    return *this; }

                inline void add(const real x)                             { const POINT p(x);     append(p); }
                inline void add(const real x, const real y)               { const POINT p(x,y);   append(p); }
                inline void add(const real x, const real y, const real z) { const POINT p(x,y,z); append(p); }

                inline virtual size_t size() const throw() { return P.size(); }

                inline point & operator[](const size_t index) throw()
                {
                    assert(index>0); assert(index<=P.size());
                    ok = false;
                    return P[index];
                }

                inline const point & operator[](const size_t index) const throw()
                {
                    assert(index>0); assert(index<=P.size());
                    return P[index];
                }

                void compute(const bool   lower_natural,
                             const point &lower_tangent,
                             const bool   upper_natural,
                             const point &upper_tangent)
                {
                    static const real half(0.5);
                    static const real six(6);
                    static const real three(3);

                    const size_t n = this->size();
                    if(ok||n<=1)
                        return;
                    else
                    {
                        Q.make(n,zp);
                        tridiag<real> t(n);
                        array<real>  &r = t._r;
                        array<real>  &u = t._u;

                        t.b[1] = t.b[n] = 1;
                        r[1]   = r[n]   = 0;
                        for(size_t i=2;i<n;++i)
                        {
                            t.a[i] = t.c[i] = 1;
                            t.b[i] = 4;
                        }
                        if(!lower_natural)
                        {
                            t.c[1] = half;
                        }
                        if(!upper_natural)
                        {
                            t.a[n] = half;
                        }
                        std::cerr << "t=" << t << std::endl;

                        for(size_t d=0;d<dim;++d)
                        {
                            r[1] = r[n] =0;
                            for(size_t i=2;i<n;++i)
                            {
                                const real  _pm = * ( (const real *)&P[i-1] + d);
                                const real  _p0 = * ( (const real *)&P[i]   + d);
                                const real  _pp = * ( (const real *)&P[i+1] + d);
                                r[i] = six * (_pm+_pp-(_p0+_p0));
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


                            std::cerr << "\tr=" << r << std::endl;
                            t.solve(u,r);
                            std::cerr << "\tu=" << u << std::endl;
                            for(size_t i=n;i>0;--i)
                            {
                                point &q = Q[i];
                                *((real *)&q + d) = u[i];
                            }
                        }
                        std::cerr << "Q=" << Q << std::endl;
                    }
                }

                inline point operator()(const real t) const
                {
                    const size_t n = this->size();

                    return zp;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(points);
                vector_type  P;   //!< points
                vector_type  Q;  //!< second derivatives
                const point  zp;
            };

        };
    }
}


#endif

