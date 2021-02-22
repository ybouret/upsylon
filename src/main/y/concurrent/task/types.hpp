//! \file

#ifndef Y_CONCURRENT_TASK_TYPE_INCLUDED
#define Y_CONCURRENT_TASK_TYPE_INCLUDED 1

#include "y/functor.hpp"

namespace upsylon
{
    namespace concurrent
    {
        class supervisor
        {
        public:
            virtual ~supervisor() throw();
            

        protected:
            explicit supervisor() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(supervisor);
        };

    }
}


#endif

