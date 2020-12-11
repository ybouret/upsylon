
#include "y/net/comm/byte.hpp"
#include "y/object.hpp"
#include "y/type/block/zset.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{
    Y_MAGAZINE_IMPL(net::comm_byte::supply,object::life_time-1)

    namespace net
    {
        comm_byte:: comm_byte(const uint8_t c) throw() :
        code(c),
        next(0),
        prev(0)
        {
        }

        comm_byte:: comm_byte() throw() : code(0), next(0), prev(0)
        {
        }
        

        comm_byte:: ~comm_byte() throw()
        {
            _bzset(code);
        }

        comm_byte::supply &comm_byte:: instance()
        {
            static supply &mgr = supply::instance();
            return mgr;
        }

        comm_byte::supply &comm_byte:: location() throw()
        {
            static supply &mgr = supply::location();
            return mgr;
        }

        std::ostream & operator<<(std::ostream &os, const comm_byte &b)
        {
            os << cchars::visible[b.code];
            return os;
        }
    }
}
