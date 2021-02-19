
//! \file

#ifndef Y_CONCURRENT_LOOP_SOLO_INCLUDED
#define Y_CONCURRENT_LOOP_SOLO_INCLUDED 1

#include "y/concurrent/loop/types.hpp"
#include "y/concurrent/sync/fake-lock.hpp"

namespace upsylon
{

    namespace concurrent
    {

        class solo : public looper
        {
        public:
            virtual ~solo() throw();
            explicit solo() throw();

            virtual void for_each(runnable &) throw();

            virtual size_t         size()                   const throw();
            virtual const context &operator[](const size_t) const throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(solo);

            fake_lock     sync;
            const context here;
        };

    }

}

#endif