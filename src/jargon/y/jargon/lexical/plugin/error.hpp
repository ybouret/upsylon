

//! \file

#ifndef Y_JARGON_LEXICAL_PLUGIN_ERROR_INCLUDED
#define Y_JARGON_LEXICAL_PLUGIN_ERROR_INCLUDED 1


#include "y/jargon/lexical/plugin.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
            
            //! try to get lexeme until next blank
            class Error : public Plugin {
            public:
                Y_JARGON_PLUGIN_DECL(Error); //!< aliases
                virtual ~Error() throw();    //!< cleanup
                
                //! setup
                template <typename ID> inline
                explicit Error(Analyzer &Lx, const ID &id) :
                Plugin(Lx,id,".",Attached), unit(0)
                {
                    setup();
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Error);
                void setup();
                auto_ptr<Unit> unit;
                virtual void  onInit(const Token &);
                void          onEmit(const Token &);
                void          onChar(const Token &);
                void          onEndl(const Token &);
                virtual void  collectEOS(string &) const;

            };
            
        }
        
    }
    
}

#endif

