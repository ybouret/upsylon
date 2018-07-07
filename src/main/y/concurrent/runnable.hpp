#ifndef Y_RUNNABLE_INCLUDED
#define Y_RUNNABLE_INCLUDED 1

#include "y/parallel.hpp"

namespace upsylon
{
    namespace concurrent
    {
        class runnable
        {
        public:
            virtual ~runnable() throw();

            virtual void run( parallel &context ) throw() = 0 ;

        protected:
            runnable() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(runnable);
        };
    }
}

#endif

