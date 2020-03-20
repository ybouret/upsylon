
//! \file
#ifndef Y_JARGON_PATTERN_OPTIONAL_INCLUDED
#define Y_JARGON_PATTERN_OPTIONAL_INCLUDED 1

#include "y/jargon/pattern/joker.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! accept any char
        class Optional : public Joker
        {
        public:
            //------------------------------------------------------------------
            //
            // definitions
            //
            //------------------------------------------------------------------
            static const uint32_t UUID = Y_FOURCC('O','P','T','_'); //!< UUID
            static const char     CLID[];                           //!< CLID
            
            //------------------------------------------------------------------
            //
            // specific methods
            //
            //------------------------------------------------------------------
            virtual         ~Optional() throw(); //!< cleanup
            static  Pattern *Create(Pattern *);  //!< create
            
        private:
            explicit Optional(const Motif &) throw();
            explicit Optional(const Optional&);
            Y_DISABLE_ASSIGN(Optional);
            virtual Pattern    *clone()                    const;
            virtual bool        alike(const Pattern *)     const throw();
            virtual bool        univocal()                 const throw();
            virtual bool        feeble()                   const throw();
            virtual bool        match(Token&,Source&)      const;
            
            virtual size_t      serialize( ios::ostream &) const;
            virtual const char *className()                const throw();
            
            virtual void        vizCore(ios::ostream &fp)  const;
        };
        
    }
    
}

#endif
