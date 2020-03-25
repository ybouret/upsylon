//! \file

#ifndef Y_JARGON_LEXICAL_PLUGIN_INCLUDED
#define Y_JARGON_LEXICAL_PLUGIN_INCLUDED 1

#include "y/jargon/lexical/scanner.hpp"

namespace upsylon {
    
    namespace Jargon
    {
        namespace Lexical
        {
            class Analyzer;
            
            class Plugin : public Scanner
            {
            public:
                virtual ~Plugin() throw();
              
                const Tag trigger;
                virtual void onInit( const Token & ) = 0;
                
                void hook( Scanner &scanner );
                
            protected:
                template <typename ID,typename EXPR>
                explicit Plugin(Analyzer       &lexer,
                                const ID       &id,
                                const EXPR     &regexp) :
                Scanner(id),
                trigger( Tags::Make(regexp) ),
                analyzer(lexer)
                {
                    Y_JSCANNER(std::cerr << "|" << id << "| @'" << regexp << "'" << std::endl;);
                }
                                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
                Analyzer &analyzer;
            };
        }
        
    }
    
}

#endif

