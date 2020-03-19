
//! \file
#ifndef Y_JARGON_PATTERN_LOGIC_AND_INCLUDED
#define Y_JARGON_PATTERN_LOGIC_AND_INCLUDED 1

#include "y/jargon/pattern/logic.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! Logic AND based patter
        class And : public Logic
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
            virtual      ~And() throw(); //!< cleanup
            static Logic *Create();      //!< create without operands
            
        
            
        private:
            explicit And() throw();
            explicit And(const And &other);
            Y_DISABLE_ASSIGN(And);
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
