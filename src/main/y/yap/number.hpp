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
