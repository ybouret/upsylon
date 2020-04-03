
//! \file

#ifndef Y_JARGON_LEXICAL_PLUGIN_ERROR_COMMENT
#define Y_JARGON_LEXICAL_PLUGIN_ERROR_COMMENT 1


#include "y/jargon/lexical/plugin.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {

            //! try to identify the unrecognized chars...
            class Error : public Plugin
            {
            public:
                virtual ~Error() throw();
                
                //! setup
                template <typename ID,
                typename ENTER> inline
                explicit Error(Analyzer    &Lx,
                               const ID    &id,
                               const ENTER &enter) :
                Plugin(Lx,id,enter), bad(0)
                {
                    setup();
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Error);
                auto_ptr<Unit> bad;
                void setup();
                virtual void onInit(const Token &);
                virtual void onGrow(const Token &);
                virtual void onDone(const Token &);
            };
        }
        
    }
}

#endif
