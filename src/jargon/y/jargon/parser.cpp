
#include "y/jargon/parser.hpp"
#include "y/exception.hpp"
#include "y/jargon/first-chars.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Parser:: ~Parser() throw()
        {
        }
        
        XNode * Parser:: parse(Module *module, const bool doAST)
        {
            Source __source( module );
            source = & __source;
            try {
                XNode *ast = accept(*this,__source,doAST);
                source = NULL;
                return ast;
            }
            catch(...)
            {
                source = NULL;
                throw;
            }
        }
        
      

        
        void Parser:: checkUnivocal(const Lexical::Rule &rule) const
        {
            if(!rule.motif->univocal())
            {
                throw exception("[%s] Terminal<%s> is not univocal", **title, **(rule.label) );
            }
        }
        
        void Parser:: compile()
        {
            //------------------------------------------------------------------
            //
            // first, validate grammar
            //
            //------------------------------------------------------------------
            validate(false);
            
            //------------------------------------------------------------------
            //
            //
            //
            //------------------------------------------------------------------
            FirstChars  fc;
            
        }

        
    }
}


