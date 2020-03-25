//! \file

#ifndef Y_JARGON_LEXICAL_PLUGIN_INCLUDED
#define Y_JARGON_LEXICAL_PLUGIN_INCLUDED 1

#include "y/jargon/lexical/scanner.hpp"

namespace upsylon {
    
    namespace Jargon
    {
        namespace Lexical
        {
            class Analyzer; //! forward declaration
            
            //! base class for a plugin
            class Plugin : public Scanner
            {
            public:
                //! cleanup
                virtual ~Plugin() throw();
              
                //! called to initialize plugin
                virtual void onInit( const Token & ) = 0;
                
                //! plugin is called upon trigger
                void hook( Scanner &scanner );
                
                //! regular expression
                const Tag trigger;

            protected:
                //! setup
                template <typename ID,typename EXPR>
                explicit Plugin(Analyzer       &Lx,
                                const ID       &id,
                                const EXPR     &regexp) :
                Scanner(id),
                trigger( Tags::Make(regexp) ),
                lexer(Lx)
                {
                    Y_JSCANNER(std::cerr << "|" << id << "| @'" << regexp << "'" << std::endl;);
                }
                                
                Analyzer &lexer; //!< used to insert compiled units
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
            };
        }
        
    }
    
}

#endif

