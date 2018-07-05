#include "y/container/container.hpp"
#include "y/type/utils.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon {

    container:: ~container() throw()
    {
    }

    container:: container() throw()
    {
    }


    size_t container:: next_capacity(const size_t n)
    {
        size_t m = max_of<size_t>(n,8);
        m += (n>>1);
        m = Y_ROUND8(m);
        if(m<=n) throw libc::exception(EDOM,"container estimate_next_capacity overflow");
        return m;
    }

}
