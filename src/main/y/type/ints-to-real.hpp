//! \file

#ifndef Y_TYPE_INTS_TO_REAL_INCLUDED
#define Y_TYPE_INTS_TO_REAL_INCLUDED 1

#include "y/type/int2int.hpp"
#include "y/exceptions.hpp"
#include <cfloat>

namespace upsylon {

    namespace kernel {

        //______________________________________________________________________
        //
        //
        //! radix
        //
        //______________________________________________________________________
        struct i2r_common
        {
            static const unsigned radix = FLT_RADIX; //!< radix
            static void check();                     //!< check radix==2
            static void out_of_range();              //!< for type conversion
        };

        //______________________________________________________________________
        //
        //
        //! forward declaration
        //
        //______________________________________________________________________
        template <typename T> struct i2r_info;

        //______________________________________________________________________
        //
        //
        //! integer to float info
        //
        //______________________________________________________________________
        template <>
        struct i2r_info<float>
        {
            typedef int32_t       itype;                   //!< matching size
            static const unsigned mantissa = FLT_MANT_DIG; //!< float mantissa
        };

        //______________________________________________________________________
        //
        //
        //! integer to double info
        //
        //______________________________________________________________________
        template <>
        struct i2r_info<double>
        {
            typedef int64_t       itype;                   //!< matching size
            static const unsigned mantissa = DBL_MANT_DIG; //!< double mantissa
        };


    }


    //__________________________________________________________________________
    //
    //
    //! checked conversion of an integer to a real
    //
    //__________________________________________________________________________
    template <typename T>
    struct i2r
    {
        //______________________________________________________________________
        //
        //! types and definition
        //______________________________________________________________________
        typedef kernel::i2r_info<T>       info_type;                  //!< fetch corresponding info
        typedef typename info_type::itype itype;                      //!< and integer type
        static  const itype __one__ = 1;                              //!< alias
        static  const itype maximum = (__one__<<info_type::mantissa); //!< alias
        static  const itype minimum = -maximum;                       //!< alias

        //______________________________________________________________________
        //
        //! V -> itype -> T, which boundaries check
        //______________________________________________________________________
        template <typename V> static inline
        T _(const V &value)
        {
            const itype i = i2i<itype,V>(value);
            if(i<minimum||maximum<i) kernel::i2r_common::out_of_range();
            const T     r = static_cast<T>(i);
            assert( static_cast<itype>(r) == i );
            return r;
        }


    };

}

#endif

