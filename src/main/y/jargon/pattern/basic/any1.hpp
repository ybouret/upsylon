//! \file
#ifndef Y_JARGON_PATTERN_ANY1_INCLUDED
#define Y_JARGON_PATTERN_ANY1_INCLUDED 1

#include "y/jargon/pattern/basic.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! accept any char
        class Any1 : public Basic
        {
        public:
            //------------------------------------------------------------------
            //
            // definitions
            //
            //------------------------------------------------------------------
            static const uint32_t UUID = Y_PATTERN_CC3('A','N','Y'); //!< UUID
            static const char     CLID[];                            //!< CLID
            
            //------------------------------------------------------------------
            //
            // specific methods
            //
            //------------------------------------------------------------------
            virtual         ~Any1() throw(); //!< cleanup
            static  Pattern *Create();       //!< create a new pattern
            
        private:
            explicit Any1() throw();
            Y_DISABLE_COPY_AND_ASSIGN(Any1);
            virtual Pattern    *clone()                    const;
            virtual bool        alike(const Pattern *)     const throw();
            virtual bool        good(const uint8_t)        const throw();
            virtual bool        univocal()                 const throw();
            
            virtual size_t      serialize( ios::ostream &) const;
            virtual const char *className()                const throw();
            virtual void        vizCore(ios::ostream &fp)  const;
        };
        
    }
    
}

#endif

