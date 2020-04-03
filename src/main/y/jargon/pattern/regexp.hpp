//! \file

#ifndef Y_JARGON_PATTER_REGEXP_INCLUDED
#define Y_JARGON_PATTER_REGEXP_INCLUDED 1

#include "y/strfwd.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Dictionary;
        class Pattern;
        
        //! regular expression engine
        /**
         group : '(', ')'
         alt   :   '|'
         joker : '?', '+', '*', '~'
         dot   : '.'
         block : '[', ']' with caret and dash...
         */
        struct RegularExpression
        {
            class Engine; //!< forward declaration
            
            static bool Verbose; //!< global debug flag, default is false
            
            
            //! compile with optional dict
            static Pattern *Compile(const string &rx, const Dictionary * =0);
            
            //! compile with optional dict
            static Pattern *Compile(const char   *rx, const Dictionary * =0);
            
            //! compile as single...
            static Pattern *Compile(const char   C, const Dictionary * =0);
            
            static const char Integer[];
            static const char Real[];
            static const char Identifier[];
        };
        
       
      
    }
}

#endif

