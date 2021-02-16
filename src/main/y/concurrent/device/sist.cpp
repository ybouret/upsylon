


#include "y/concurrent/device/sist.hpp"

namespace upsylon {

    namespace concurrent
    {

        sist:: ~sist() throw()
        {}


        sist:: sist() throw() : impl()
        {
        }

        const engine & sist:: bulk() const throw()
        {
            return impl;
        }

        void sist:: loop(executable code,void *args)
        {
            assert(code);
            code(args,impl[1],impl.sync());
        }

    }

}


