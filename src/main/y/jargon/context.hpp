
//! \file

#ifndef Y_JARGON_CONTEXT_INCLUDED
#define Y_JARGON_CONTEXT_INCLUDED 1

#include "y/jargon/types.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! precise localisation of content
        class Context
        {
        public:
            const Stamp    stamp;   //!< shared stamp
            const unsigned line;    //!< current line of content
            const unsigned column;  //!< current column of content
            
            explicit Context(const Stamp  &) throw(); //!< setup line=1, column=1
            explicit Context(const string &);         //!< setup from string
            explicit Context(const char   *);         //!< setup from text
            Context(const Context &) throw();         //!< no-throw copy
            virtual ~Context() throw();               //!< cleanup
            
        private:
            Y_DISABLE_ASSIGN(Context);
        };
    }
    
}

#endif
