
#include "y/net/io/byte.hpp"
#include "y/object.hpp"
#include "y/type/block/zset.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{
    Y_MAGAZINE_IMPL(net::io_byte::supply,object::life_time-1)

    namespace net
    {
        io_byte:: io_byte(const uint8_t c) throw() :
        code(c),
        next(0),
        prev(0)
        {
        }

        io_byte:: ~io_byte() throw()
        {
            _bzset(code);
        }

        io_byte::supply &io_byte:: instance()
        {
            static supply &mgr = supply::instance();
            return mgr;
        }

        io_byte::supply &io_byte:: location() throw()
        {
            static supply &mgr = supply::location();
            return mgr;
        }

        std::ostream & operator<<(std::ostream &os, const io_byte &b)
        {
            os << cchars::visible[b.code];
            return os;
        }
    }
}
