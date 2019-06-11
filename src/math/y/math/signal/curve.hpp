//! \file

#ifndef Y_SIGNAL_CURVE_INCLUDED
#define Y_SIGNAL_CURVE_INCLUDED 1

#include "y/type/point3d.hpp"
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

                inline explicit points() throw() : P(), P2(), zp(0) {}
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

                inline virtual void compute(const bool   lower_natural,
                                            const point &lower_slope,
                                            const bool   upper_natural,
                                            const point &upper_slope)
                {
                    static const real half  = real(0.5);
                    static const real three = real(3);

                    if(ok)
                    {
                        return;
                    }
                    else
                    {
                        const size_t n = P.size();
                        if(n>1)
                        {
                            
                        }
                        ok = true;
                    }
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(points);
                vector_type  P;   //!< points
                vector_type  P2;  //!< second derivatives
                vector_type  U;   //!< for internal computation
                const point  zp;
            };

        };
    }
}


#endif

