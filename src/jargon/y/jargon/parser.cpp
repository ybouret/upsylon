
#include "y/jargon/parser.hpp"
#include "y/exception.hpp"
#include "y/jargon/first-chars.hpp"
#include "y/ios/icstream.hpp"

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
        
      

        
        void Parser:: checkUnivocal(const Lexical::Rule &rule, const char *which) const
        {
            if(!rule.motif->univocal())
            {
                throw exception("[%s] %s<%s> is not univocal", **title, which, **(rule.label) );
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

        void Parser:: cleanup() throw()
        {
            cache->release();
            xcache.release();
        }
        
        XNode *Parser:: treeFromFile(Context &where)
        {
            ios::icstream input( *(where.tag) );
            return loadTree(where,cache,input);
        }

        
    }
}


