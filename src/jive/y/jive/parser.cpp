
#include "y/jive/parser.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Jive
    {

        Parser:: ~Parser() throw()
        {
        }

        
        XNode * Parser:: parse(Source &source,const size_t prefetch)
        {
            ready(source,prefetch);
            XNode::Pointer xnode( run(*this,source) );
            return xnode.yield();
        }

        const Lexical::Rule & Parser:: checkStrong(const Lexical::Rule &rule) const
        {
            if(!rule.motif->strong())
            {
                throw exception("%s has weak terminal <%s>",**name,**rule.label);
            }
            return rule;
        }

        Syntax::Terminal::Type Parser:: queryTypeOf(const Lexical::Rule &rule) const
        {
            assert(rule.motif->strong());
            return rule.motif->univocal() ? Terminal::Univocal : Terminal::Standard;
        }

    }

}

