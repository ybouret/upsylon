
#include "y/ios/omstream.hpp"
#include "y/exception.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace ios
    {

        omstream:: ~omstream() throw()
        {
            _bzset(space);
            _bzset(count);
            _bzset(entry);
        }

        omstream:: omstream( void *addr, const size_t size ) throw() :
        count(0),
        space(size),
        entry( static_cast<char *>(addr) )
        {
            assert(!(NULL==entry&&space>0));

        }

        omstream:: omstream(  memory::rw_buffer &buff ) throw() :
        count(0),
        space( buff.length() ),
        entry( static_cast<char *>(buff.rw()) )
        {
            assert(!(NULL==entry&&space>0));
        }

        void omstream:: write( char C )
        {
            if(count>=space) throw exception("ios::omstream(count>space=%u)", unsigned(space));
            entry[ aliasing::_(count )++ ] = C;
        }

        void omstream:: flush() {}
    }

}

