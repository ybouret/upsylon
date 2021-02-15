
//! \file

#ifndef Y_TYPE_INTS_UTILS_INCLUDED
#define Y_TYPE_INTS_UTILS_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/os/static-check.hpp"

namespace upsylon {

    namespace core
    {

        struct count_digits
        {

            template <typename T>
            static inline size_t base10(const T &x) throw()
            {
                static const int2type< is_signed<T>::value > selector = {};
                return base10<T>(x,selector);
            }

            //! unsigned
            template <typename T>
            static inline size_t base10(T value, int2type<false> ) throw()
            {
                Y_STATIC_CHECK(false==is_signed<T>::value,signed_type);
                size_t count = 0;
                do
                {
                    ++count;
                    value /= 10;

                } while(value>0);
                return count;
            }

            //! signed
            template <typename T>
            static inline size_t base10(T value, int2type<true> ) throw()
            {
                Y_STATIC_CHECK(true==is_signed<T>::value,signed_type);
                size_t count = 0;
                if(value<0)
                {
                    ++count;
                    value = - value;
                }
                do
                {
                    ++count;
                    value /= 10;

                } while(value>0);
                return count;
            }


        };


    }

}

#endif
