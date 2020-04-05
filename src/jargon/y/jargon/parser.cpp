
#include "y/jargon/parser.hpp"
#include "y/exception.hpp"
#include "y/jargon/first-chars.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Parser:: ~Parser() throw()
        {
        }
        
        XNode * Parser:: parse(Module *module)
        {
            Source __source( module );
            source = & __source;
            try {
                XNode *ast = accept(*this,__source);
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
            validate(false);
            assert(alpha.size>0);
            FirstChars  fc;
            for(const AlphaNode *anode=alpha.head;anode;anode=anode->next)
            {
                fc.free();
                const Axiom    &axiom = anode->axiom;
                const TermList &terms = anode->terms; assert(terms.size>0);
                for(const TermNode *tnode=terms.head;tnode;tnode=tnode->next)
                {
                    getRule(tnode->axiom.label).motif->adjoin(fc);
                }
                std::cerr << "for <" << axiom.label << "> : " << fc << "" << std::endl;
                
            }
        }

        
    }
}


