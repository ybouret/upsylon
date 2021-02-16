//! \file

#ifndef Y_CONCURRENT_SIST_INCLUDED
#define Y_CONCURRENT_SIST_INCLUDED 1

#include "y/concurrent/device/looper.hpp"
#include "y/concurrent/device/solo.hpp"

namespace upsylon {

    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        //! single instruction single thread
        //
        //______________________________________________________________________
        class sist
        {
        public:
            explicit sist() throw(); //!< setup
            virtual ~sist() throw(); //!< cleanup

            virtual void loop(executable code, void *args); //!< solo call
            virtual void join() throw();                    //!< do nothing

        private:
            Y_DISABLE_COPY_AND_ASSIGN(sist);
            virtual const engine & bulk() const throw();
            solo impl;
        };

    }
}

#endif
