

//! \file

#ifndef Y_JIVE_LEXICAL_PLUGIN_RSTRING_INCLUDED
#define Y_JIVE_LEXICAL_PLUGIN_RSTRING_INCLUDED 1

#include "y/jive/lexical/plugin/string_.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {
            //__________________________________________________________________
            //
            //
            //! raw strings, between single quotes
            //
            //__________________________________________________________________
            class rString : public String_
            {
            public:
                //! cleanup
                virtual ~rString() throw();
                
                //! setup
                template <typename ID> inline
                explicit rString(const ID &id, Queue &q) :
                String_(id,quote ,quote,q)
                {
                }
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(rString);
            };
            
        }
        
    }
}

#endif

