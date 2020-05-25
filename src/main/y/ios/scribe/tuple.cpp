
#include "y/ios/scribe/tuple.hpp"

namespace upsylon {

    namespace ios {

        tuple_scribe:: ~tuple_scribe() throw()
        {
        }

        tuple_scribe:: tuple_scribe(const std::type_info &t,
                                    const size_t          n,
                                    const basic_ptr      &h) :
        scribe(t,n),
        handle(h)
        {
        }
    }

}


