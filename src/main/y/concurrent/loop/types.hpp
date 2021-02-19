//! \file

#ifndef Y_CONCURRENT_LOOP_TYPES_INCLUDED
#define Y_CONCURRENT_LOOP_TYPES_INCLUDED 1

#include "y/concurrent/loop/runnable.hpp"
#include "y/sequence/accessible.hpp"

namespace upsylon
{

    namespace concurrent
    {

        class looper : public accessible<context>
        {
        public:
            virtual ~looper() throw();

            virtual void loop(runnable &) throw() = 0;
            virtual void join()           throw() = 0;

            void         for_each(runnable&) throw();

            double       efficiency(const double speed_up) const throw();

        protected:
            explicit looper() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(looper);
        };

    }

}

#endif
