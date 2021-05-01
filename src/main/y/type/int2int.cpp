
#include "y/type/int2int.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon {

    namespace core {

        void _i2i:: overflow_exception(const rtti &target,
                                       const rtti &source)
        {
            throw libc::exception(ERANGE,"overflow in %s -> %s", source.text(), target.text());
        }

        void _i2i:: negative_exception(const rtti &target,
                                       const rtti &source)
        {
            throw libc::exception(ERANGE,"negative value in %s -> %s", source.text(), target.text());
        }

        

    }

}
