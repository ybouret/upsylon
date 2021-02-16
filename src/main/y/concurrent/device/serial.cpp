

#include "y/concurrent/device/serial.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace concurrent
    {

        serial:: ~serial() throw()
        {
        }

        serial:: serial() throw() :
        synchronize(),
        solo(),
        done(false)
        {
        }

        size_t serial:: size() const throw()
        {
            return 1;
        }

        lockable & serial:: sync() throw()
        {
            return synchronize;
        }

        void serial:: once(executable code, void *args)
        {
            assert(code);
            if(done) throw exception("concurrent::serial already done");
            done = true;
            code(args,solo,synchronize);
        }
    }

}

