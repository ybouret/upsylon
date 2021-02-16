

#include "y/concurrent/device/solo.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace concurrent
    {

        solo:: ~solo() throw()
        {
        }

        solo:: solo() throw() :
        synchronize(),
        here(),
        done(false)
        {
        }

        size_t solo:: size() const throw()
        {
            return 1;
        }

        lockable & solo:: sync() throw()
        {
            return synchronize;
        }

        const context & solo:: operator[](const size_t) const throw()
        {
            return here;
        }

        void solo:: once(executable code, void *args)
        {
            assert(code);
            if(done) throw exception("concurrent::serial already done");
            done = true;
            code(args,here,synchronize);
        }
    }

}

