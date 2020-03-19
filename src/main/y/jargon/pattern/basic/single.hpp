
//! \file
#ifndef Y_JARGON_PATTERN_SINGLE_INCLUDED
#define Y_JARGON_PATTERN_SINGLE_INCLUDED 1

#include "y/jargon/pattern/basic.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Single : public Basic
        {
        public:
            static const uint32_t UUID = Y_PATTERN_CC4('S','N','G','L');
            static const char     CLID[];
            
            virtual        ~Single() throw();
            static Pattern *Create(const uint8_t c);
            
            const uint8_t code;
            
            
        protected:
            explicit Single(const uint8_t) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Single);
            virtual Pattern    *clone()                    const;
            virtual bool        alike(const Pattern *)     const throw();
            virtual bool        good(const uint8_t)        const throw();
            virtual bool        univocal()                 const throw();
            
            virtual size_t      serialize( ios::ostream &) const;
            virtual const char *className()                const throw();
        };
        
    }
    
}

#endif

