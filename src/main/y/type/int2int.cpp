
#include "y/type/int2int.hpp"
#include "y/exceptions.hpp"
#include "y/type/spec.hpp"
#include <cerrno>

namespace upsylon {

    namespace core {

        void _i2i:: overflow_exception(const type_spec &target,
                                       const type_spec &source)
        {
            throw libc::exception(ERANGE,"overflow in %s -> %s", *(source.name()), *(target.name()));
        }

        

    }

}
