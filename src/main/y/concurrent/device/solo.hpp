
//! \file

#ifndef Y_CONCURRENT_SOLO_INCLUDED
#define Y_CONCURRENT_SOLO_INCLUDED 1

#include "y/concurrent/device/engine.hpp"
#include "y/concurrent/fake-lock.hpp"

namespace upsylon {

    namespace concurrent
    {

        class solo : public engine
        {
        public:
            virtual ~solo() throw();
            explicit solo() throw();

            virtual lockable & sync()       throw();
            virtual size_t     size() const throw();
            virtual void       once(executable,void*);



        private:
            Y_DISABLE_COPY_AND_ASSIGN(solo);
            fake_lock     synchronize;
            const context here;
            bool          done;
        };

    }
}

#endif
