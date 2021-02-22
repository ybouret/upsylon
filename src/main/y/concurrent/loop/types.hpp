//! \file

#ifndef Y_CONCURRENT_LOOP_TYPES_INCLUDED
#define Y_CONCURRENT_LOOP_TYPES_INCLUDED 1

#include "y/concurrent/loop/runnable.hpp"
#include "y/sequence/accessible.hpp"

namespace upsylon
{

    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        //! interface to make a loop on a runnable object
        //
        //______________________________________________________________________
        class looper : public accessible<context>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~looper() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual void for_each(runnable &) throw() = 0; //!< run on contexts

            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________

            //! efficiency based on size()
            double       efficiency(const double speed_up) const throw();

        protected:
            explicit looper() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(looper);
        };

    }

}

#endif
