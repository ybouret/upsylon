//! \file

#ifndef Y_JARGON_FIRST_CHARS_INCLUDED
#define Y_JARGON_FIRST_CHARS_INCLUDED 1

#include "y/ordered/sorted-vector.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon {
 
    namespace Jargon {
     
        typedef sorted_vector<
        uint8_t,
        increasing_comparator<uint8_t>,
        memory::pooled
        > SortedChars;
    
        typedef ordered_single<SortedChars> FirstCharsType;
        
        class FirstChars : public FirstCharsType
        {
        public:
            explicit FirstChars(size_t n=0);
            virtual ~FirstChars() throw();
            
            void loadAll();
            
            friend std::ostream & operator<<(std::ostream &, const FirstChars &);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(FirstChars);
        };
        
    }
    
}

#endif

