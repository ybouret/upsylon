//! \file

#ifndef Y_JARGON_FIRST_CHARS_INCLUDED
#define Y_JARGON_FIRST_CHARS_INCLUDED 1

#include "y/ordered/sorted-vector.hpp"
#include "y/memory/allocator/pooled.hpp"

namespace upsylon {
 
    namespace Jargon {
     
        //! base class for ordered first chars
        typedef sorted_vector<uint8_t,increasing_comparator<uint8_t>,memory::pooled> SortedChars;
    
        //! base class for first chars
        typedef ordered_single<SortedChars> FirstCharsType;
        
        //! a set of possible first chars for a pattern
        class FirstChars : public FirstCharsType
        {
        public:
            explicit FirstChars() throw(); //!< initialize
            explicit FirstChars(size_t n); //!< initialize with capacity
            virtual ~FirstChars() throw(); //!< cleanup
            
            void loadAll(); //!< [0..255]
            
            //! dedicated display
            friend std::ostream & operator<<(std::ostream &, const FirstChars &);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(FirstChars);
        };
        
    }
    
}

#endif

