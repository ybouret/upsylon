//! \file
#ifndef Y_AP_NUMBER_INCLUDED
#define Y_AP_NUMBER_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/code/ilog2.hpp"
#include "y/ptr/counted.hpp"

namespace upsylon {

    namespace yap {

//#define Y_YAP_FORCE16


        //______________________________________________________________________
        //
        //
        //! base class for numbers
        //
        //______________________________________________________________________
        class number : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            // integral types
            typedef uint64_t utype; //!< user unsigned integral type
            typedef int64_t  itype; //!< user signed   integral type

            // core type, to perform internal computations
            typedef typename unsigned_int<sizeof(void*)>::type core_type;
            static const size_t                                core_size = sizeof(core_type);
            static const size_t                                core_bits = core_size << 3;


            // word type, to handle memory
#if defined(Y_YAP_FORCE16)
            typedef uint16_t                                    word_type;
#else
            typedef typename unsigned_int<(core_size>>1)>::type word_type;
#endif
            static const size_t                                word_size = sizeof(word_type);
            static const size_t                                word_bits = word_size << 3;
            static const size_t                                word_exp2 = ilog2<word_size>::value;

          
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
