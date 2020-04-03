
#include "y/jargon/parser.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Parser:: ~Parser() throw()
        {
        }
        
        XNode * Parser:: parse(Source &source)
        {
            return accept(*this,source);
        }
        
        void Parser:: checkUnivocal(const Lexical::Rule &rule) const
        {
            if(!rule.motif->univocal())
            {
                throw exception("[%s] Terminal<%s> is not univocal", **title, **(rule.label) );
            }
        }
        
    }
}


