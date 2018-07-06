//! \file
#ifndef Y_CONCURRENT_SIMD_INCLUDED
#define Y_CONCURRENT_SIMD_INCLUDED 1

#include "y/concurrent/threads.hpp"

namespace upsylon
{
    namespace concurrent
    {

        class simd : public threads
        {
        public:
            explicit simd(const bool v=false);
            virtual ~simd() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(simd);
            bool quit;
        };
    }
}

#endif
