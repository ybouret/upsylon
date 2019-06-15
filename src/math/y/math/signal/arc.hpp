//! \file
#ifndef Y_MATH_ARC_INCLUDED
#define Y_MATH_ARC_INCLUDED 1

#include "y/sequence/vector.hpp"
#include "y/math/types.hpp"
#include "y/type/point3d.hpp"
#include "y/math/kernel/cyclic.hpp"
#include "y/ios/ostream.hpp"

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
                inline virtual ~spline() throw() {}

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

            protected:
                inline explicit spline() throw() : vector<POINT>(), origin(0) {}

                virtual void __compute( array<POINT> &Q, const array<POINT> &P ) = 0;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(spline);

            public:
                const POINT origin;

            };

            template <typename POINT>
            class standard_spline : public spline<POINT>
            {
            public:
                bool  lower_natural;
                bool  upper_natural;
                POINT lower_tangent;
                POINT upper_tangent;
                POINT S1;
                POINT SN;

                inline explicit standard_spline() throw() :
                spline<POINT>(),
                lower_natural(true),
                upper_natural(true),
                lower_tangent(0),
                upper_tangent(0),
                S1(0),
                SN(0)
                {}


            private:
                Y_DISABLE_COPY_AND_ASSIGN(standard_spline);
                virtual void __compute(  array<POINT> &Q, const array<POINT> &P )
                {
                    assert( P.size() > 1 );
                    assert( Q.size() == P.size() );

                    const size_t n = P.size();


                }
            };

            template <typename POINT>
            class periodic_spline : public spline<POINT>
            {
            public:

                inline explicit periodic_spline() throw() :
                spline<POINT>()
                {}


            private:
                Y_DISABLE_COPY_AND_ASSIGN(periodic_spline);
                virtual void __compute(  array<POINT> &Q, const array<POINT> &P )
                {
                    assert( P.size() > 1 );
                    assert( Q.size() == P.size() );

                    const size_t n = P.size();
                    
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

