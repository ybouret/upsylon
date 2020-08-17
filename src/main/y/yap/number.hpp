//! \file
#ifndef Y_AP_NUMBER_INCLUDED
#define Y_AP_NUMBER_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/code/ilog2.hpp"
#include "y/ptr/counted.hpp"
#include "y/ios/serializable.hpp"

namespace upsylon {

    namespace yap {

//#define Y_YAP_FORCE16



        //______________________________________________________________________
        //
        //
        //! base class for numbers
        //
        //______________________________________________________________________
        class number : public object, public counted, public ios::serializable
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //------------------------------------------------------------------
            // integral types
            //------------------------------------------------------------------
            typedef uint64_t utype; //!< user unsigned integral type
            typedef int64_t  itype; //!< user signed   integral type

            //------------------------------------------------------------------
            // core type, to perform internal computations
            //------------------------------------------------------------------
            typedef typename unsigned_int<sizeof(void*)>::type core_type;                     //!< system native unsigned
            static const size_t                                core_size = sizeof(core_type); //!< system native size
            static const size_t                                core_bits = core_size << 3;    //!< system native  bits


            //------------------------------------------------------------------
            // word type, to handle memory
            //------------------------------------------------------------------
#if defined(Y_YAP_FORCE16)
            typedef uint16_t                                    word_type; //!< memory of word_type
#else
            typedef typename unsigned_int<(core_size>>1)>::type word_type; //!< memory of word_type
#endif
            static const size_t                                word_size = sizeof(word_type);       //!< word size
            static const size_t                                word_bits = word_size << 3;          //!< word bits
            static const size_t                                word_exp2 = ilog2<word_size>::value; //!< word_size = 1 << word_exp2
            static const size_t                                word_mask = word_size-1;             //!< word_size - 1 = least significant bits

            //! number of words per utype
            static const size_t                                words_per_utype = sizeof(utype)/word_size;

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~number() throw(); //!< cleanup

        protected:
            explicit number() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(number);
        };
    }

}

#endif
