//! \file

#ifndef Y_TYPE_INTS_TO_REAL_INCLUDED
#define Y_TYPE_INTS_TO_REAL_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/exceptions.hpp"
#include <cfloat>

namespace upsylon {

    namespace kernel {


        struct i2r_radix
        {
            static const unsigned value = FLT_RADIX;
            static void check();
        };
        
        template <typename T> struct i2r_info;

        template <>
        struct i2r_info<float>
        {
            typedef int32_t       itype;
            static const unsigned mantissa = FLT_MANT_DIG;
        };

        template <>
        struct i2r_info<double>
        {
            typedef int64_t       itype;
            static const unsigned mantissa = DBL_MANT_DIG;
        };

        template <typename T>
        struct i2r
        {
            typedef i2r_info<T>               info_type;
            typedef typename info_type::itype itype;
            static  const itype __one__ = 1;
            static  const itype minimum = -(__one__<<info_type::mantissa);
            static  const itype maximum = (-minimum)+1;
        };
        
    }
}

#endif

