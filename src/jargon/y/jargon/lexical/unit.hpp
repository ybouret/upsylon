//! \file

#ifndef Y_JARGON_LEXICAL_UNIT_INCLUDED
#define Y_JARGON_LEXICAL_UNIT_INCLUDED 1

#include "y/jargon/token.hpp"

namespace upsylon {
 
    class exception;
    
    namespace Jargon {
     
        namespace Lexical {
         
            //! a token created by a lexical rule
            class Unit :
            public Object,
            public Token,
            public Context,
            public inode<Unit>
            {
            public:
                typedef core::list_of_cpp<Unit> List; //!< alias for Lexemes
                
                virtual ~Unit() throw(); //!< cleanup
                
                //! setup
                explicit Unit(const Context &,
                              const Tag     &) throw();
                
                
                void append(const uint8_t code);  //!< append one char
                void append(const string &data);  //!< append a string
                void append(const Token  &token); //!< append a token
                
                const Tag label;                         //!< creating rule label
                void  writeTo(exception &excp,
                              const bool definite)const; //!< append to exception
                
            private:
                Y_DISABLE_ASSIGN(Unit);
            };
            
        }
    }
    
}

#endif
