
#ifndef SUPPORT_INCLUDED
#define SUPPORT_INCLUDED 1


#include "y/mpl/rational.hpp"
#include "y/type/complex.hpp"
#include "y/type/point3d.hpp"
#include "y/alea.hpp"
#include "y/string.hpp"

using namespace upsylon;

namespace {


    struct support
    {

        template <typename T>
        static inline T get()
        {
            return alea.template full<T>();
        }

    };

    template <>
    inline string support:: get<string>()
    {
        const size_t len = 1+ alea.leq(63);
        string ans(len,as_capacity,false);
        for(size_t i=0;i<len;++i)
        {
            ans += alea.range<char>('a','z');
        }
        return ans;
    }

    template <>
    inline float support:: get<float>()
    {
        return alea.to<float>();
    }

    template <>
    inline double support:: get<double>()
    {
        return alea.to<double>();
    }

    template <>
    inline mpn support:: get<mpn>()
    {
        return mpn( alea.leq(30), alea );
    }
    
    template <>
    inline mpz support:: get<mpz>()
    {
        const mpn n( alea.leq(30), alea );
        return mpz( alea.choice() ? mpl::__negative : mpl::__positive, n );
    }
    
    template <>
    inline mpq support:: get<mpq>()
    {
        const mpz num = support::get<mpz>();
        while(true)
        {
            const mpn den = support::get<mpn>();
            if( !den.is_zero() )
            {
                return mpq(num,den);
            }
        }
    }

    template <>
    inline complex<float> support:: get< complex<float> >()
    {
        return complex<float>( get<float>(), get<float>() );
    }


    template <>
    inline complex<double> support:: get< complex<double> >()
    {
        return complex<double>( get<double>(), get<double>() );
    }

    template <>
    inline point2d<float> support:: get< point2d<float> >()
    {
        return alea.on_circle< float,point2d >();
    }

    template <>
    inline point2d<double> support:: get< point2d<double> >()
    {
        return alea.on_circle< double,point2d >();
    }

    template <>
    inline point3d<float> support:: get< point3d<float> >()
    {
        return alea.on_sphere< float,point3d >();
    }

    template <>
    inline point3d<double> support:: get< point3d<double> >()
    {
        return alea.on_sphere< double,point3d >();
    }

    
}

#endif

