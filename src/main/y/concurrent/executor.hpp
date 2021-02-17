//! \file
#ifndef Y_CONCURRENT_EXECUTOR_INCLUDED
#define Y_CONCURRENT_EXECUTOR_INCLUDED 1

#include "y/parallel.hpp"
#include "y/concurrent/sync/fake-lock.hpp"

namespace upsylon
{
    namespace concurrent
    {
        //__________________________________________________________________
        //
        //
        //! a function that may be run in parallel
        //
        //__________________________________________________________________
        typedef void (*kernel)(void *, parallel &, lockable &);

        //__________________________________________________________________
        //
        //
        //! interface to run kernels
        //
        //__________________________________________________________________
        class executor : public parallel::group
        {
        public:
            //__________________________________________________________________
            //
            //
            // virtual interface
            //
            //__________________________________________________________________
            //! destructor
            virtual           ~executor() throw();

            //! run a kernel with data, presenting a context and a shared access
            virtual void       run(kernel, void *) = 0;


            //__________________________________________________________________
            //
            //
            // flat memory per thread
            //
            //__________________________________________________________________

            void acquire(const size_t);   //!< acquire bytes for all contexts
            void free()    throw();       //!< for all contexts
            void release() throw();       //!< for all contexts

           
            

        protected:
            //! constructor
            explicit  executor() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(executor);
        };

        //! implements a sequential executor
        class sequential : public executor
        {
        public:
            explicit sequential() throw();             //!< constructor
            virtual ~sequential() throw();             //!< destructor
            virtual void             run(kernel proc, void *);               //!< direct call
            virtual size_t           size()                   const throw(); //!< only one
            virtual parallel       & operator[](const size_t) throw();       //!< [1]
            virtual const parallel & operator[](const size_t) const throw(); //!< [1]

        private:
            parallel  context;

        public:
            fake_lock access; //!< shared access

        };

    }
}

#endif

