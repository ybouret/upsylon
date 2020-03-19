
//! \file
#ifndef Y_JARGON_PATTERN_RANGE_INCLUDED
#define Y_JARGON_PATTERN_RANGE_INCLUDED 1

#include "y/jargon/pattern/basic.hpp"


namespace upsylon {
    
    namespace Jargon {
        
        class Range : public Basic
        {
        public:
            static const uint32_t UUID = Y_PATTERN_CC3('R','N','G');
            static const char     CLID[];
            
            virtual        ~Range() throw();
            static Pattern *Create(const uint8_t a, const uint8_t b);
            
            const uint8_t lower;
            const uint8_t upper;
            
        protected:
            explicit Range(const uint8_t,const uint8_t) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Range);
            virtual Pattern    *clone() const;
            virtual bool        alike(const Pattern *) const throw();
            virtual size_t      serialize( ios::ostream &) const;
            virtual const char *className() const throw();
            virtual bool        good(const uint8_t) const throw();
        };
        
    }
    
}

#endif

