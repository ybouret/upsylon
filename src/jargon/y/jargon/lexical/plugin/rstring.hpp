
//! \file

#ifndef Y_JARGON_LEXICAL_PLUGIN_RSTRING_COMMENT
#define Y_JARGON_LEXICAL_PLUGIN_RSTRING_COMMENT 1


#include "y/jargon/lexical/plugin/strings.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
            
            //! raw string, single quote separated
            class rString : public Strings
            {
            public:
                virtual ~rString() throw(); //!< cleanup
                
                //! setup
                template <typename ID> inline
                rString(Analyzer &Lx, const ID   &id) : Strings(Lx,id,Separator) {}
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(rString);
                static const char Separator;
            };
            
        }
        
    }
    
}

#endif

