
#ifndef SUPPORT_INCLUDED
#define SUPPORT_INCLUDED 1


#include "y/yap/rational.hpp"
#include "y/type/complex.hpp"
#include "y/type/point3d.hpp"
#include "y/alea.hpp"
#include "y/string.hpp"
#include "y/os/real-time-clock.hpp"

using namespace upsylon;

namespace {

#define Y_SUPPORT_TICKS(NAME,CODE) uint64_t NAME = 0; do { const uint64_t ini = rt_clock::ticks(); CODE; NAME = rt_clock::ticks()-ini; } while(false)

    struct support
    {

        template <typename T>
        static inline T get()
        {
            return alea.template full<T>();
        }

        template <typename ARRAY>
        static inline void fill1D( ARRAY &arr )
        {
            for(size_t i=arr.size();i>0;--i)
            {
                arr[i] = get< typename ARRAY::mutable_type >();
            }
        }

        template <typename ARRAY>
        static inline void reset1D( ARRAY &arr )
        {
            typename ARRAY::const_type z = 0;
            for(size_t i=arr.size();i>0;--i)
            {
                arr[i] = z;
            }
        }


        template <typename ARRAY>
        static inline void fill2D( ARRAY &arr )
        {
            for(size_t i=arr.rows;i>0;--i)
            {
                fill1D(arr[i]);
            }
        }
        
        template <typename ARRAY>
        static inline void fill3D( ARRAY &arr )
        {
            for(size_t i=arr.slices;i>0;--i)
            {
                fill2D(arr[i]);
            }
        }
    };

    template <>
    inline bool support::get<bool>()
    {
        return alea.choice();
    }

    template <>
    inline string support:: get<string>()
    {
        const size_t len = 1+ alea.leq(16);
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
    inline apn support:: get<apn>()
    {
        return apn( alea,alea.leq(30) );
    }
    
    template <>
    inline apz support:: get<apz>()
    {
        return apz( alea, alea.leq(30) );
    }
    
    template <>
    inline apq support:: get<apq>()
    {
        return apq(alea,alea.leq(30),alea.leq(30));
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
    inline point2d<int> support:: get< point2d<int> >()
    {
        return point2d<int>( alea.full<int>(), alea.full<int>() );
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

    template <>
    inline point3d<long long> support:: get< point3d<long long> >()
    {
        return point3d<long long>( alea.full<long long>(), alea.full<long long>(), alea.full<long long>() );
    }
    
}

#endif

