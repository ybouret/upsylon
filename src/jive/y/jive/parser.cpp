
#include "y/jive/parser.hpp"

namespace upsylon
{

    namespace Jive
    {

        Parser:: ~Parser() throw()
        {
        }

        void Parser:: start() throw()
        {
            reset();
        }


        XNode * Parser:: parse(Source &source,const size_t prefetch)
        {
            ready(source,prefetch);
            XNode::Pointer xnode( run(*this,source) );
            return xnode.yield();
        }

    }

}

