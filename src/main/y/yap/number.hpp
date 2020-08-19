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

        //! type for integer of comparisons
        enum sign_type
        {
            __negative, //!< -1
            __zero,     //!<  0
            __positive  //!<  1
        };

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
           
            typedef uint64_t utype; //!< user unsigned integral type
            typedef int64_t  itype; //!< user signed   integral type

            static sign_type sign_of(const itype) throw();
            static utype     iabs_of(const itype) throw();

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
