//! \file
#ifndef Y_CONCURRENT_FOR_EACH_INCLUDED
#define Y_CONCURRENT_FOR_EACH_INCLUDED 1

#include "y/concurrent/executor.hpp"

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
        class for_each
        {
        public:
            
            //! destructor
            virtual ~for_each() throw();

            //! launch kernel(s)
            virtual void   start( kernel , void * ) = 0;

            //! wait for all kernels to return
            virtual void   finish() throw()         = 0;

            //! get underlying engine
            virtual executor & engine() throw()    = 0;

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
            
            //! call the kernel on data
            virtual void start( kernel , void * );

            //! here, do nothing..
            virtual void finish() throw();

            //! return the engine
            virtual executor & engine() throw();

        private:
            sequential engine_;            
            Y_DISABLE_COPY_AND_ASSIGN(sequential_for);
        };


    }
}

#endif

