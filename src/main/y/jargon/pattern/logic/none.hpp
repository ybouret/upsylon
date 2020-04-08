


//! \file
#ifndef Y_JARGON_PATTERN_LOGIC_NONE_INCLUDED
#define Y_JARGON_PATTERN_LOGIC_NONE_INCLUDED 1

#include "y/jargon/pattern/logic.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! Logical NONE based pattern
        //
        //----------------------------------------------------------------------
        class NONE : public Logical
        {
        public:
            //------------------------------------------------------------------
            //
            // definitions
            //
            //------------------------------------------------------------------
            static const uint32_t UUID = Y_FOURCC('N','O','N','E'); //!< UUID
            static const char     CLID[];                           //!< CLID
            
            //------------------------------------------------------------------
            //
            // specific methods
            //
            //------------------------------------------------------------------
            virtual        ~NONE() throw();     //!< cleanup
            static Logical *Create();           //!< create without operands
            
            
        private:
            explicit NONE() throw();
            explicit NONE(const NONE &other);
            Y_DISABLE_ASSIGN(NONE);
            virtual  Pattern    *clone()                    const;
            virtual  bool        alike(const Pattern *)     const throw();
            virtual  bool        univocal()                 const throw();
            virtual  bool        feeble()                   const throw();
            virtual  bool        match(Token &, Source &)   const;
            virtual  void        adjoin( FirstChars &)      const;

            virtual const char  *className()                const throw();
            virtual void         vizCore(ios::ostream &fp)  const;
        };
        
    }
    
}


#endif
