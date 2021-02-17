//! \file

#ifndef Y_CONCURRENT_ENGINE_INCLUDED
#define Y_CONCURRENT_ENGINE_INCLUDED 1

#include "y/concurrent/context.hpp"
#include "y/sequence/accessible.hpp"
#include "y/object.hpp"

namespace upsylon {

    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! engine interface to run one time code
        //
        //______________________________________________________________________
        class engine : public object, public accessible<context>
        {
        public:
            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual lockable & sync()       throw()   = 0; //!< internal lock
            virtual void       once(executable,void*) = 0; //!< run once the code

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~engine() throw(); //!< cleanup
        protected:
            explicit engine() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(engine);
        };

    }
}

#endif