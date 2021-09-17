
//! \file

#ifndef YAP_GCD_INCLUDED
#define YAP_GCD_INCLUDED 1

#include "y/yap/integer.hpp"
#include "y/sequence/list.hpp"

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
                list<T>  todo;
                for(size_t i=arr.size();i>0;--i)
                {
                    const T a = mkl::fabs_of(arr[i]);
                    if(a>0) todo.push_back(a);
                }

                if(todo.size())
                {
                    T res = todo.back(); todo.pop_back();
                    if(one==res) return one;
                    while(todo.size())
                    {
                        res = from(res,todo.back()); todo.pop_back();
                        if(one==res) return one;
                    }
                    return res;
                }
                else
                    return one;
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
