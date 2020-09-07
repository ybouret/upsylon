
//! \file
#ifndef Y_JARGON_PATTERN_LOGIC_AND_INCLUDED
#define Y_JARGON_PATTERN_LOGIC_AND_INCLUDED 1

#include "y/jargon/pattern/logic.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! Logical AND based pattern
        //
        //----------------------------------------------------------------------
        class AND : public Logical
        {
        public:
            //------------------------------------------------------------------
            //
            // definitions
            //
            //------------------------------------------------------------------
            static const uint32_t UUID = Y_FOURCC('A','N','D','_'); //!< UUID
            static const char     CLID[];                           //!< CLID
            
            //------------------------------------------------------------------
            //
            // specific methods
            //
            //------------------------------------------------------------------
            virtual         ~AND() throw();      //!< cleanup
            static  Logical *Create();           //!< create without operands
            
            
        private:
            explicit AND() throw();
            explicit AND(const AND &other);
            Y_DISABLE_ASSIGN(AND);
            virtual  Pattern    *clone()                    const;
            virtual  bool        alike(const Pattern *)     const throw();
            virtual  bool        univocal()                 const throw();
            virtual  bool        feeble()                   const throw();
            virtual  bool        match(Token &, Source &)   const; 
            virtual  void        adjoin( FirstChars &)      const;
            virtual void        express(ios::ostream &)    const;

            virtual const char  *className()                const throw();
            virtual void         vizCore(ios::ostream &fp)  const;
        };
        
    }
    
}


#endif
