
//! \file

#ifndef YAP_GCD_INCLUDED
#define YAP_GCD_INCLUDED 1

#include "y/yap/integer.hpp"
#include "y/sequence/addressable.hpp"

namespace upsylon
{
    namespace yap
    {
        //______________________________________________________________________
        //
        //
        //! computing least common multiple
        //
        //______________________________________________________________________
        struct compute_gcd
        {
            //! check and use natural::gcd
            static natural from(const natural &lhs, const natural &rhs);

            //! positive gcd of absolute values
            static integer from(const integer &lhs, const integer &rhs);

            //! for apn/apz
            template <typename T> static inline
            T of(const accessible<T> &arr)
            {
                const  T one(1);
                size_t   n = arr.size();
                switch(n)
                {
                    case 1:
                    {
                        const T &x = arr[1];
                        if(0!=x) return x;
                    } // FALLTHRU
                    case 0: return one;

                    default:
                        break;
                }
                assert(n>1);
                T res = from(arr[1],arr[n]);
                if(one==res) return one;
                for(--n;n>1;--n)
                {
                    res = from(res,arr[n]);
                    if(one==res) return one;
                }
                return res;
            };


            //! for apn/apz
            template <typename T> static inline
            T simplify(addressable<T> &arr)
            {
                const T fac = of<T>(arr);
                for(size_t i=arr.size();i>0;--i)
                {
                    arr[i] /= fac;
                }
                return fac;
            }



        };

    }

}

#endif
