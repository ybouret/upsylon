//! \file
#ifndef Y_CONCURRENT_FOR_EACH_INCLUDED
#define Y_CONCURRENT_FOR_EACH_INCLUDED 1

#include "y/parallel.hpp"
#include "y/concurrent/fake-lock.hpp"
#include "y/dynamic.hpp"

namespace upsylon
{
    namespace concurrent
    {

        //! for each semantic
        /**
         It will apply the same kernel to different portions of data.
         The parallel context is provided to compute where to compute,
         and a lockable object is provided for synchronization.
         */
        class for_each : public virtual dynamic
        {
        public:
            typedef void (*kernel)( void *, parallel &, lockable & ); //!< the kernel prototype

            //! destructor
            virtual ~for_each() throw();

            //! launch kernel(s)
            virtual void   start( kernel , void * ) = 0;
            //! wait for all kernels to return
            virtual void   finish() throw()         = 0;
            //! access to context
            virtual parallel & operator[](const size_t) throw() = 0;
            //! access to context
            virtual const parallel & operator[](const size_t) const throw() = 0;

        protected:
            //! constructor
            explicit for_each() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(for_each);
        };


        //! sequential implementation for for_each interface
        class sequential_for : public for_each
        {
        public:
            virtual ~sequential_for() throw(); //!< destructor
            explicit sequential_for() throw(); //!< destructor

            inline virtual size_t size()     const throw() { return 1; }
            inline virtual size_t capacity() const throw() { return 1; }

            //! call the kernel on data
            virtual void start( kernel , void * );
            //! here, do nothing..
            virtual void finish() throw();
            //! the parallel context
            inline virtual parallel       & operator[](const size_t) throw()       { return context; }
            //! the context
            inline virtual const parallel & operator[](const size_t) const throw() { return context; }


        private:
            parallel  context;
            fake_lock my_lock;
            
            Y_DISABLE_COPY_AND_ASSIGN(sequential_for);
        };


    }
}

#endif

