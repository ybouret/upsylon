//! \file

#ifndef Y_JARGON_PATTER_REGEXP_INCLUDED
#define Y_JARGON_PATTER_REGEXP_INCLUDED 1

#include "y/strfwd.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Dictionary;
        class Pattern;
        
        struct RegularExpression
        {
            class Engine; //!< forward declaration
            
            static bool Verbose;
            static Pattern *Compile_(const string &rx, const Dictionary * =0);
            static Pattern *Compile_(const char   *rx, const Dictionary * =0);
            static Pattern *Compile(const string &rx, const Dictionary * =0);
            static Pattern *Compile(const char   *rx, const Dictionary * =0);
            
        };
        
       
      
    }
}

#endif

