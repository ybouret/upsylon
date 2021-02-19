
//! \file

#ifndef Y_CONCURRENT_EXECUTABLE_INCLUDED
#define Y_CONCURRENT_EXECUTABLE_INCLUDED 1

#include "y/concurrent/thread.hpp"
#include "y/concurrent/sync/mutex.hpp"
#include "y/concurrent/topology.hpp"


namespace upsylon
{

    namespace concurrent
    {

        //! function to execute on a context (i.e. within a thread)
        class executable
        {
        public:
            
            //! launcher to redirect executable.call
            class launcher : public thread
            {
            public:
                virtual ~launcher() throw();
                explicit launcher(executable  &user_host,
                                  const size_t user_size,
                                  const size_t user_rank);
            private:
                Y_DISABLE_COPY_AND_ASSIGN(launcher);
                executable &host;
                static void stub(void *) throw();
            };


            virtual ~executable() throw();
            virtual void call(const context &) = 0;

            mutex             access;
            topology::pointer topo;

        protected:
            explicit executable();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(executable);
        };

    }

}

#endif

