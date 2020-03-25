//! \file

#ifndef Y_JARGON_LEXICAL_PLUGIN_JSTRING_COMMENT
#define Y_JARGON_LEXICAL_PLUGIN_JSTRING_COMMENT 1


#include "y/jargon/lexical/plugin/strings.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {

            //! a double quote delimited string
            class jString : public Strings
            {
            public:
                virtual ~jString() throw(); //!< cleanup
               
                //! setup
                template <typename ID> inline
                jString(Analyzer &Lx, const ID   &id) : Strings(Lx,id,Separator) {}
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(jString);
                static const char Separator;
            };
        }
        
    }
    
}

#endif

