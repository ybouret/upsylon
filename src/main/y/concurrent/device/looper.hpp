//! \file

#ifndef Y_CONCURRENT_LOOPER_INCLUDED
#define Y_CONCURRENT_LOOPER_INCLUDED 1

#include "y/concurrent/context.hpp"
#include "y/concurrent/device/engine.hpp"
#include "y/type/gateway.hpp"

namespace upsylon {

    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! looper interface for single instruction
        //
        //______________________________________________________________________
        class looper : public gateway<engine>
        {
        public:
            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual void loop(executable code, void *args) = 0; //!< start a new cycle

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~looper() throw(); //!< cleanup

        protected:
            explicit looper() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(looper);

        };

    }
}

#endif
