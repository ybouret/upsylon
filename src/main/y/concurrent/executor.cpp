
#include "y/concurrent/executor.hpp"

namespace upsylon
{
    namespace concurrent
    {
        executor:: ~executor() throw() {}
        executor::  executor() throw() {}

        void executor:: make_all(const size_t n)
        {
            //std::cerr << "alloc/clear space for " << n <<  " bytes" << std::endl;
            executor    &self = *this;
            const size_t nthr = num_threads();
            for(size_t i=0;i<nthr;++i)
            {
                self[i].make(n);
            }
        }

        void executor:: free_all() throw()
        {
            executor &self = *this;
            size_t    i    = num_threads();
            while(i-->0)
            {
                self[i].free();
            }
        }

    }
}
