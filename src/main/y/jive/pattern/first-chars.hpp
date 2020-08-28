//! \file

#ifndef Y_JIVE_PATTERN_FIRST_CHARS_INCLUDED
#define Y_JIVE_PATTERN_FIRST_CHARS_INCLUDED 1

#include "y/memory/allocator/pooled.hpp"
#include "y/ordered/sorted-vector.hpp"

namespace upsylon
{

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        // types used in first chars
        //
        //______________________________________________________________________
        typedef increasing_comparator<uint8_t>                  fcComparator; //!< alias
        typedef memory::pooled                                  fcAllocator;  //!< alias
        typedef sorted_vector<uint8_t,fcComparator,fcAllocator> fcSequence;   //!< alias
        typedef ordered_single<fcSequence>                      fcOrdered;    //!< alias


        //______________________________________________________________________
        //
        //
        //! First possible chars to start with for a pattern
        //
        //______________________________________________________________________
        class FirstChars : public fcOrdered
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit FirstChars();          //!< setup
            virtual ~FirstChars() throw();  //!< cleanup
            FirstChars(const FirstChars &); //!< copy

            void complete();                     //!< set all 256 possibilites
            void opposite(const FirstChars &fc); //!< opposite chars

            //! smart display
            friend std::ostream & operator<<(std::ostream &, const FirstChars &fc );

        private:
            Y_DISABLE_ASSIGN(FirstChars);

        };
    }

}

#endif
