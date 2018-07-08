
//! \file
#ifndef Y_CONCURRENT_EXECUTOR_INCLUDED
#define Y_CONCURRENT_EXECUTOR_INCLUDED 1

#include "y/parallel.hpp"
#include "y/concurrent/fake-lock.hpp"

namespace upsylon
{
    namespace concurrent
    {
        typedef void (*kernel)(void *, parallel &, lockable &);

        //! interface to run kernels
        class executor
        {
        public:
            virtual  ~executor() throw();


            virtual void       run( kernel, void * ) = 0;
            virtual size_t     num_threads()   const throw() = 0;
            virtual parallel & operator[](const size_t) throw() = 0;

        protected:
            explicit  executor() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(executor);
        };

        //! implements a sequential exectuor
        class sequential : public executor
        {
        public:
            //! constructor
            inline explicit sequential() throw() :
            executor(),context(),access() {}

            //! destructor
            inline virtual ~sequential() throw() {}

            //! direct call
            inline virtual void run( kernel proc, void *data )
            {
                assert(proc);
                proc(data,context,access);
            }

            //! only one
            inline virtual size_t num_threads() const throw() { return 1; }

            //! context
            inline virtual parallel & operator[](const size_t) throw() { return context; }
            
        private:
            parallel  context;
            fake_lock access;

        };

    }
}

#endif

