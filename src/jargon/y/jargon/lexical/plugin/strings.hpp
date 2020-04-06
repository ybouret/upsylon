
//! \file

#ifndef Y_JARGON_LEXICAL_PLUGIN_STRINGS_COMMENT
#define Y_JARGON_LEXICAL_PLUGIN_STRINGS_COMMENT 1


#include "y/jargon/lexical/plugin.hpp"

namespace upsylon
{
    namespace Jargon {
        
        namespace Lexical {
            
            //! [r|j]String
            class Strings : public Plugin
            {
            public:
                virtual ~Strings() throw(); //!< cleanup
                

            protected:
                //! setup
                template <typename ID> inline
                Strings(Analyzer   &Lx,
                        const ID   &id,
                        const char  C) :
                Plugin(Lx,id,C),
                content(0),
                context(0),
                symbol(C)
                {
                    setup();
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Strings);
                void setup();
                virtual void onInit(const Token &);
                auto_ptr<string>  content;
                auto_ptr<Context> context;
                const char        symbol;
                
                void onCore(const Token &);
                void onEmit(const Token &);
                void onEsc0(const Token &); // \nrtvbf
                void onEsc1(const Token &); // quote, dquote, slash, backslash
                void onHexa(const Token &); // \xXX
                void failed(const Token &); // error
                
                void checkSymbol(const Token &sep, const char *fn) const;
            };
            
          
            
           
            
        }
        
    }
    
}

#endif
