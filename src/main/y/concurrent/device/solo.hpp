
//! \file

#ifndef Y_CONCURRENT_SOLO_INCLUDED
#define Y_CONCURRENT_SOLO_INCLUDED 1

#include "y/concurrent/device/engine.hpp"
#include "y/concurrent/fake-lock.hpp"

namespace upsylon {

    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! solo engine to test code
        //
        //______________________________________________________________________
        class solo : public engine
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~solo() throw(); //!< cleanup
            explicit solo() throw(); //!< setup

            //__________________________________________________________________
            //
            // engine
            //__________________________________________________________________
            virtual lockable &     sync()       throw();   //!< a fake lock
            virtual void           once(executable,void*); //!< local run

            //__________________________________________________________________
            //
            // accessible<context>
            //__________________________________________________________________
            virtual size_t         size() const throw(); //!< 1
            virtual const context &operator[](const size_t) const throw(); //!< only context


        private:
            Y_DISABLE_COPY_AND_ASSIGN(solo);
            fake_lock     synchronize;
            const context here;
            bool          done;
        };

    }
}

#endif
