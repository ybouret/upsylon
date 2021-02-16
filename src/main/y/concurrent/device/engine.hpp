//! \file

#ifndef Y_CONCURRENT_ENGINE_INCLUDED
#define Y_CONCURRENT_ENGINE_INCLUDED 1

#include "y/concurrent/context.hpp"
#include "y/object.hpp"

namespace upsylon {

    namespace concurrent
    {

        class engine : public object
        {
        public:
            virtual ~engine() throw();

            virtual lockable & sync()       throw()   = 0;
            virtual size_t     size() const throw()   = 0;
            virtual void       once(executable,void*) = 0;

        protected:
            explicit engine() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(engine);
        };

    }
}

#endif
