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
        struct curve
        {
            //! template to extract info from types
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
            
            template <> struct info_for< complex<float> >
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

            template <> struct info_for< complex<double> >
            {
                static const size_t dim = 2;
                typedef double      real;
            };
            
            template <> struct info_for< point3d<double> >
            {
                static const size_t dim = 3;
                typedef double      real;
            };

            class interface
            {
            public:
                const size_t dimensions;
                const size_t real_bytes;
                const size_t point_size; //!< dimensions * real_bytes
                
                virtual ~interface() throw();
                virtual size_t      size() const throw() = 0;
                virtual const void *addr(const size_t index) const throw() = 0;
                
                void save_point( ios::ostream &fp, const void *p ) const;
                void save( ios::ostream &fp ) const;
                
                //! 'time' to 'index' to output points coordinate
                template <typename T>
                T t2i( const T t ) const throw()
                {
                    const size_t n = size();
                    const size_t nm1 = n-1;
                    return clamp<T>(1,T(1)+ t * nm1,n);
                }
                
                
                
            protected:
                explicit interface(const size_t d, const size_t r) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(interface);
            };
            
            template <typename T>
            class interface_for : public interface
            {
            public:
                inline virtual ~interface_for() throw() {}
                
            protected:
                explicit interface_for( const size_t d ) throw() : interface(d, sizeof(T)) {}
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(interface_for);
            };
            
            template <
            typename POINT,
            typename ALLOCATOR= memory::global
            >
            class points : public interface_for< typename info_for<POINT>::real >
            {
            public:
                typedef info_for<POINT>          info_type;
                static const size_t              dim = info_type::dim;
                typedef POINT                    point;
                typedef typename info_type::real real;
                typedef vector<point,ALLOCATOR>  vector_type;

                inline explicit points() throw() :
                interface_for<real>(dim), P(), Q(), zp(0),
                lower_natural(true),
                upper_natural(true),
                lower_tangent(0),
                upper_tangent(0)
                {}
                
                inline virtual ~points() throw() {}


                inline void append( const point &p )         { P.push_back(p);}
                inline points & operator<<( const point &p ) { append(p); return *this; }

                inline void add(const real x)                             { const POINT p(x);     append(p); }
                inline void add(const real x, const real y)               { const POINT p(x,y);   append(p); }
                inline void add(const real x, const real y, const real z) { const POINT p(x,y,z); append(p); }

                inline virtual size_t size() const throw() { return P.size(); }
                
                inline const point & operator[](const size_t index) const throw()
                {
                    assert(index>0); assert(index<=P.size());
                    return P[index];
                }

                virtual const void *addr(const size_t index) const throw()
                {
                    assert(index>0); assert(index<=P.size());
                    return  &P[index];
                }

                
                virtual void compute()
                {
                    static const real half(0.5);
                    static const real six(6);
                    static const real three(3);
                    static const real one(1);
                    static const real four(4);
                    
                    const size_t n = this->size();
                    if(n<=1)
                    {
                        Q.free();
                        return;
                    }
                    else
                    {
                        Q.make(n,zp);
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
                                point &q = Q[i];
                                *((real *)&q + d) = u[i];
                            }
                        }
                    }
                }

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

            private:
                Y_DISABLE_COPY_AND_ASSIGN(points);
                vector_type  P;   //!< points
                vector_type  Q;   //!< second derivatives
            public:
                const point  zp;
                bool  lower_natural;
                bool  upper_natural;
                point lower_tangent;
                point upper_tangent;
    
            };

        };
    }
}


#endif

