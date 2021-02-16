


//! \file

#ifndef Y_CONCURRENT_SIMT_INCLUDED
#define Y_CONCURRENT_SIMT_INCLUDED 1

#include "y/concurrent/device/looper.hpp"
#include "y/concurrent/device/crew.hpp"

namespace upsylon {

    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! single instruction multiple threads
        //
        //______________________________________________________________________
        class simt : public looper
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit simt();          //!< create crew, prepare loop
            virtual ~simt() throw();  //!< cleanup

            virtual void loop(executable code, void *args);



        private:
            Y_DISABLE_COPY_AND_ASSIGN(simt);
            virtual const engine & bulk() const throw();
            crew impl;
        };

    }
}

#endif
