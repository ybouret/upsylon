#include "y/counting/permuter.hpp"
#include "y/type/block/zset.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon {

    namespace core {

        permuter:: ~permuter() throw()
        {
            _bzset(dims);
            _bzset(perm);
        }

        permuter:: permuter(const size_t n) :
        dims(n),
        perm(0)
        {
            if(dims<=0) throw libc::exception( ERANGE, "permuter(0)" );
        }



    }

}
