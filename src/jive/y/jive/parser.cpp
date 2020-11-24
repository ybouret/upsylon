
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

        void Parser:: checkStrong(const Lexical::Rule &rule) const
        {
            if(!rule.motif->strong())
            {
                throw exception("%s has weak terminal <%s>",**name,**rule.label);
            }
        }
    }

}

