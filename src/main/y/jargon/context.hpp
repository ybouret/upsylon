
//! \file

#ifndef Y_JARGON_CONTEXT_INCLUDED
#define Y_JARGON_CONTEXT_INCLUDED 1

#include "y/jargon/types.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //------------------------------------------------------------------
        //
        //! precise localisation of content
        //
        //------------------------------------------------------------------
        class Context
        {
        public:
            const Tag      tag;     //!< shared tag
            const int      line;    //!< current line of content
            const int      column;  //!< current column of content
            
            Context( const Tag    &);
            Context( const string &);
            Context( const char   *);
            Context(const Context &) throw();         //!< no-throw copy
            virtual ~Context()       throw();         //!< cleanup
            
        private:
            Y_DISABLE_ASSIGN(Context);
        };
    }
    
}

#endif
