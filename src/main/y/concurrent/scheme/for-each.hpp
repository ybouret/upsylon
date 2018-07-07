//! \file

#ifndef Y_CONCURRENT_FOR_EACH_INCLUDED
#define Y_CONCURRENT_FOR_EACH_INCLUDED 1

#include "y/parallel.hpp"
#include "y/concurrent/fake-lock.hpp"

namespace upsylon
{
    namespace concurrent
    {

        class for_each
        {
        public:
            typedef void (*kernel)( void *, parallel &, lockable & ); //!< the kernel prototype
            
            virtual ~for_each() throw();

            //virtual size_t num_threads() const throw() = 0;
            virtual parallel & operator[](size_t) throw() =0;
            virtual void   start( kernel , void * ) = 0;
            virtual void   finish() throw()         = 0;

        protected:
            explicit for_each() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(for_each);
        };


        //! sequential implementation for for_each interface
        class sequential_for : public for_each
        {
        public:
            virtual ~sequential_for() throw();
            explicit sequential_for() throw();

            virtual void start( kernel , void * );
            virtual void finish() throw();
            virtual parallel & operator[](size_t) throw();
            
        private:
            parallel  context;
            fake_lock my_lock;
            
            Y_DISABLE_COPY_AND_ASSIGN(sequential_for);
        };


    }
}

#endif

