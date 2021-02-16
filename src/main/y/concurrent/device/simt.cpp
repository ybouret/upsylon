

#include "y/concurrent/device/simt.hpp"

namespace upsylon {

    namespace concurrent
    {

        simt:: ~simt() throw()
        {}


        simt:: simt() : impl()
        {
        }

        const engine & simt:: bulk() const throw()
        {
            return impl;
        }

        void simt:: loop(executable code, void *args)
        {
            assert(code);
        }

    }

}


