//! \file

#ifndef Y_JIVE_PATTERN_FIRST_CHARS_INCLUDED
#define Y_JIVE_PATTERN_FIRST_CHARS_INCLUDED 1

#include "y/memory/allocator/pooled.hpp"
#include "y/ordered/sorted-vector.hpp"

namespace upsylon
{

    namespace Jive
    {
        typedef increasing_comparator<uint8_t>                  fcComparator;
        typedef memory::pooled                                  fcAllocator;
        typedef sorted_vector<uint8_t,fcComparator,fcAllocator> fcSequence;
        typedef ordered_single<fcSequence>                      fcOrdered;

        class FirstChars : public fcOrdered
        {
        public:
            explicit FirstChars();
            virtual ~FirstChars() throw();

            void fulfill();

        private:
            Y_DISABLE_ASSIGN(FirstChars);

        };
    }

}

#endif
