
#ifndef SUPPORT_INCLUDED
#define SUPPORT_INCLUDED 1

#include "y/mpl/rational.hpp"
#include "y/alea.hpp"

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
        const size_t len = 1+ alea.leq(15);
        string ans(len,as_capacity);
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
        return mpn( alea.leq(100), alea );
    }
    
    template <>
    inline mpz support:: get<mpz>()
    {
        const mpn n( alea.leq(100), alea );
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
    
    
}

#endif

