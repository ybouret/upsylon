
//! \file

#ifndef Y_CONCURRENT_EXECUTABLE_INCLUDED
#define Y_CONCURRENT_EXECUTABLE_INCLUDED 1

#include "y/concurrent/thread.hpp"
#include "y/concurrent/component.hpp"


namespace upsylon
{

    namespace concurrent
    {

        

        //______________________________________________________________________
        //
        //
        //! function to execute on a context (i.e. within a thread)
        //
        //______________________________________________________________________
        class executable : public component
        {
        public:
            //__________________________________________________________________
            //
            //! launcher to redirect executable.call
            //__________________________________________________________________
            class launcher : public thread
            {
            public:
                virtual ~launcher() throw();               //!< cleanup
                explicit launcher(executable  &user_host,  //|
                                  const size_t user_size,  //|
                                  const size_t user_rank); //!< setup

            protected:
                executable &program; //!< persistent program

            private:
                Y_DISABLE_COPY_AND_ASSIGN(launcher);
                static void stub(void *) throw(); // host.call(*this)
            };

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual     ~executable() throw();      //!< cleanup
            virtual void call(const context &) = 0; //!< called for each thread

            
        protected:
            explicit executable();    //!< setup topology

        private:
            Y_DISABLE_COPY_AND_ASSIGN(executable);
        };

    }

}

#endif

