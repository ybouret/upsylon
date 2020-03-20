

//! \file
#ifndef Y_JARGON_PATTERN_LOGIC_OR_INCLUDED
#define Y_JARGON_PATTERN_LOGIC_OR_INCLUDED 1

#include "y/jargon/pattern/logic.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! Logic AND based patter
        class OR : public Logical
        {
        public:
            //------------------------------------------------------------------
            //
            // definitions
            //
            //------------------------------------------------------------------
            static const uint32_t UUID = Y_FOURCC('O','R','_','_'); //!< UUID
            static const char     CLID[];                           //!< CLID
            
            //------------------------------------------------------------------
            //
            // specific methods
            //
            //------------------------------------------------------------------
            virtual        ~OR() throw();       //!< cleanup
            static Logical *Create();           //!< create without operands
            virtual void    simplify() throw(); //!< remove redundant
            
            
        private:
            explicit OR() throw();
            explicit OR(const OR &other);
            Y_DISABLE_ASSIGN(OR);
            virtual  Pattern    *clone()                    const;
            virtual  bool        alike(const Pattern *)     const throw();
            virtual  bool        univocal()                 const throw();
            virtual  bool        feeble()                   const throw();
            virtual  bool        match(Token &, Source &)   const;
            
            virtual const char  *className()                const throw();
            virtual void         vizCore(ios::ostream &fp)  const;
        };
        
    }
    
}


#endif
