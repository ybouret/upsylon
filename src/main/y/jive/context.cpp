
#include "y/jive/context.hpp"
#include "y/type/block/zset.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jive
    {

        Context:: ~Context() throw()
        {
            _bzset(line);
            _bzset(column);

        }

#define Y_JIVE_CTX(args) tag(args), line(init_line), column(init_column)

        Context:: Context(const Tag &t) throw() :
        Y_JIVE_CTX(t)
        {
        }

        Context:: Context(const char *s) :
        Y_JIVE_CTX( new string(s) )
        {
        }

        Context:: Context(const string &s) :
        Y_JIVE_CTX( new string(s) )
        {
        }





        Context:: Context(const Context &other) throw() :
        tag(other.tag),
        line(other.line),
        column(other.column)
        {
        }

        exception & Context:: stampTo(exception &excp) const throw()
        {
            excp.cat("%s:%d:%d: ", **tag,line,column);
            return excp;
        }

        
    }

}


