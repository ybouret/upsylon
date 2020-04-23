
#include "y/jargon/parser.hpp"
#include "y/exception.hpp"
#include "y/jargon/first-chars.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/imstream.hpp"

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
        
        
        
        XNode *Parser:: treeFromFile(Context &where)
        {
            ios::icstream input( *(where.tag) );
            return loadTree(where,input);
        }
        
        XNode * Parser:: treeFromData(Context &where, const void *data, const size_t size)
        {
            ios::imstream input(data,size);
            return loadTree(where,input);
        }

        
        const Pattern & Parser:: patternOf(const Axiom &axiom) const
        {
            
            switch(axiom.uuid)
            {
                case Terminal::UUID:
                case Operator::UUID:
                    break;
                    
                default:
                    throw exception("[%s] no pattern for <%s> [%s]", **title, **(axiom.label), fourcc_(axiom.uuid));
            }
            
            const Lexical::Rule *r = getByLabel(axiom.label);
            if(!r)
                throw exception("[%s] no lexical rule for <%s> [%s]", **title, **(axiom.label), fourcc_(axiom.uuid));
            return  *(r->motif);
        }
        
        
        
        
    }
}

#include "y/ios/ocstream.hpp"
#include "y/ios/tools/graphviz.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        void Parser:: graphVizPatterns(const string &fn) const
        {
            do
            {
                ios::ocstream gv(fn);
                gv << "digraph G{\n";
                unsigned i = 0;
                for(Axioms::const_iterator it = axioms.begin(); it != axioms.end(); ++it )
                {
                    const Axiom         &axiom = **it;
                    const Lexical::Rule *rule  = getByLabel(axiom.label);
                    if(!rule) continue;
                    gv("subgraph cluster_%u{\n",++i);
                    gv << "\tstyle=solid;";
                    gv << "\tlabel=\"";
                    gv << (**axiom.label);
                    gv << "\";\n";
                    rule->motif->vizSave(gv);
                    gv("}\n");
                    //rule->motif->graphViz(fileName);
                }
                gv << "}\n";
            } while(false);
            ios::GraphViz::Render(fn);
        }
        
        void Parser:: graphVizPatterns(const char *fn) const
        {
            const string _(fn); graphVizPatterns(_);
        }
        
    }
    
}

