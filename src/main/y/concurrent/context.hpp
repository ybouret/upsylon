//! \file

#ifndef Y_CONCURRENT_CONTEXT_INCLUDED
#define Y_CONCURRENT_CONTEXT_INCLUDED 1

#include "y/parops.hpp"

namespace upsylon
{

    namespace concurrent
    {

        //! MPI style context for threads
        class context
        {
        public:
            explicit context() throw(); //! size=1
            explicit context(const size_t sz,
                             const size_t rk) throw();
            virtual ~context() throw();

            const size_t size; //!< size>=1
            const size_t rank; //!< 0<=rank<size
            const size_t indx; //!< rank+1
            const char   label[16];

        private:
            Y_DISABLE_ASSIGN(context);
            void fmt() throw();
        };

    }

}

#endif

