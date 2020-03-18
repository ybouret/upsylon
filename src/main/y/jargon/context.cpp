
#include "y/jargon/context.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        Context:: ~Context() throw()
        {
            _bzset(line);
            _bzset(column);
        }
        
#define Y_JCTX(S) stamp(S),line(1),column(1)
        
        Context:: Context(const Stamp  &s) throw() : Y_JCTX(s)     { }
        Context:: Context(const string &s) : Y_JCTX(new string(s)) { }
        Context:: Context(const char   *s) : Y_JCTX(new string(s)) { }

        Context:: Context(const Context &other) throw() :
        stamp( other.stamp ),
        line(  other.line  ),
        column(other.column)
        {
            assert(line>0);
            assert(column>0);
        }
        
        
        
        
    }
    
}
