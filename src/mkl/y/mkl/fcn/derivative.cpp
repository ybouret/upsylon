#include "y/mkl/fcn/derivative.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
namespace upsylon {

    namespace mkl {

        namespace kernel {

            void derivative:: underflow()
            {
                 throw libc::exception(EDOM,"derivative underflow");
            }

            void derivative:: invalid_at(const double x)
            {
                libc::exception(EDOM, "invalid derivative @%.15g", x);
            }

        }

    }

}
