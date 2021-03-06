
//! \file

#ifndef Y_TYPE_INTS_UTILS_INCLUDED
#define Y_TYPE_INTS_UTILS_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/os/static-check.hpp"

namespace upsylon {

    namespace core
    {

        //______________________________________________________________________
        //
        //
        //! counting digits of integer types
        //
        //______________________________________________________________________
        struct count_digits
        {

            //__________________________________________________________________
            //
            //! for any integer
            //__________________________________________________________________
            template <typename T>
            static inline unsigned base10(const T &x) throw()
            {
                static const int2type< is_signed<T>::value > selector = {};
                return base10<T>(x,selector);
            }

            //__________________________________________________________________
            //
            //! for unsigned integer
            //__________________________________________________________________
            template <typename T>
            static inline unsigned base10(T value, int2type<false> ) throw()
            {
                Y_STATIC_CHECK(false==is_signed<T>::value,signed_type);
                unsigned count = 0;
                do
                {
                    ++count;
                    value /= 10;

                } while(value>0);
                return count;
            }

            //__________________________________________________________________
            //
            //! for signed integer
            //__________________________________________________________________
            template <typename T>
            static inline unsigned base10(T value, int2type<true> ) throw()
            {
                Y_STATIC_CHECK(true==is_signed<T>::value,signed_type);
                unsigned count = 0;
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
