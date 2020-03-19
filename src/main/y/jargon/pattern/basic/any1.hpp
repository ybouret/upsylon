//! \file
#ifndef Y_JARGON_PATTERN_ANY1_INCLUDED
#define Y_JARGON_PATTERN_ANY1_INCLUDED 1

#include "y/jargon/pattern/basic.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Any1 : public Basic
        {
        public:
            static const uint32_t UUID = Y_PATTERN_CC3('A','N','Y');
            static const char     CLID[];
            
            virtual         ~Any1() throw();
            static  Pattern *Create();
            
        private:
            explicit Any1() throw();
            Y_DISABLE_COPY_AND_ASSIGN(Any1);
            virtual Pattern    *clone() const;
            virtual bool        alike(const Pattern *) const throw();
            virtual size_t      serialize( ios::ostream &) const;
            virtual const char *className() const throw();
            virtual bool        good(const uint8_t) const throw();
        };
        
    }
    
}

#endif

