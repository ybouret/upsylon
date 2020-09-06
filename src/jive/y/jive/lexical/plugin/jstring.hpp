
//! \file

#ifndef Y_JIVE_LEXICAL_PLUGIN_JSTRING_INCLUDED
#define Y_JIVE_LEXICAL_PLUGIN_JSTRING_INCLUDED 1

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
            //! Jive String, betweeen double quotes
            //
            //__________________________________________________________________
            class jString : public String_
            {
            public:
                virtual ~jString() throw(); //!< cleanup
                
                //! setup
                template <typename ID> inline
                explicit jString(const ID &id, Queue &q) :
                String_(id, '"' , q)
                {
                }
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(jString);
            };

        }
        
    }
}

#endif

