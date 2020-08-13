
//! \file

/**
 Simple Inline Bignum upsYlon Library
 */
#ifndef Y_SIBYL_NUMBER_INCLUDED
#define Y_SIBYL_NUMBER_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/ptr/counted.hpp"
#include "y/memory/tight/quarry-allocator.hpp"

namespace upsylon {


    namespace sibyl {


        typedef memory::tight::quarry_allocator &memory_allocator; //!< alias


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

            // system
            static  const size_t                         sys_core_size = sizeof(void*); //!<  system unsigned size
            typedef unsigned_int<sys_core_size>::type    sys_core_type;                 //!<  system unsigned type

            // widest
            typedef unsigned_int<sizeof(uint64_t)>::type max_core_type;                         //!< 64 bits computation ?
            static  const size_t                         max_core_size = sizeof(max_core_type); //!< its size
            static  const size_t                         max_word_size = max_core_size >> 1;    //!< maximal word size

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            static memory_allocator &instance();          //!< internal dedicated memory
            static memory_allocator &location() throw();  //!< internal dedicated memory

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~number() throw(); //!< cleanup

        protected:
            explicit number() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(number);
        };



    }

}

#endif
