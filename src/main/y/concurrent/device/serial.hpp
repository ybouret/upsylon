
//! \file

#ifndef Y_CONCURRENT_SERIAL_INCLUDED
#define Y_CONCURRENT_SERIAL_INCLUDED 1

#include "y/concurrent/device/engine.hpp"
#include "y/concurrent/fake-lock.hpp"

namespace upsylon {

    namespace concurrent
    {

        class serial
        {
        public:
            virtual ~serial() throw();

            virtual lockable & sync()       throw();
            virtual size_t     size() const throw();
            virtual void       once(executable,void*);


        protected:
            explicit serial() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(serial);
            fake_lock     synchronize;
            const context solo;
            bool          done;
        };

    }
}

#endif
