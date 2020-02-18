
#include "y/container/factory.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace core {

        const char factory:: default_name[] = "factory";

        void factory::throw_multiple_keys(const char *name)
        {
            throw exception("multiple key for [%s] creator", name ? name : default_name );
        }

        void factory:: throw_no_creator(const char *name)
        {
            throw exception("no key for creator from [%s]",name ? name : default_name );
        }

    }

}
