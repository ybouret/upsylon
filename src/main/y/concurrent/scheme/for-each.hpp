//! \file
#ifndef Y_CONCURRENT_FOR_EACH_INCLUDED
#define Y_CONCURRENT_FOR_EACH_INCLUDED 1

#include "y/concurrent/executor.hpp"
#include "y/ptr/counted.hpp"
#include "y/ptr/arc.hpp"

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
        class for_each : public counted_object
        {
        public:

            //! destructor
            virtual ~for_each() throw();

            //! run kernel(s)
            virtual void   run( kernel , void * ) = 0;


            //! get underlying engine
            virtual executor       & engine() throw()          = 0;

            //! get underlying engine, const
            virtual const executor & engine() const throw()    = 0;

            //! engine().num_threads()
            size_t number() const throw();

        protected:
            //! constructor
            explicit for_each() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(for_each);
        };

        typedef arc_ptr<for_each> shared_for_each;


        //! sequential implementation for for_each interface
        class sequential_for : public for_each
        {
        public:
            virtual ~sequential_for() throw(); //!< destructor
            explicit sequential_for() throw(); //!< destructor
            
            //! call the kernel on data
            virtual void run( kernel , void * );


            //! return the engine
            virtual executor & engine() throw();

            //! return the const engine
            virtual const executor & engine() const throw();

        private:
            sequential engine_;            
            Y_DISABLE_COPY_AND_ASSIGN(sequential_for);
        };


    }
}

#endif

