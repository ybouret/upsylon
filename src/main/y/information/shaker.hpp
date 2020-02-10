//! \file
#ifndef Y_INFORMATION_SHAKER_INCLUDED
#define Y_INFORMATION_SHAKER_INCLUDED 1

#include "y/memory/buffer.hpp"

namespace upsylon {

    namespace information {

        //! basic information shaker interface
        class shaker
        {
        public:

            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------
            virtual        ~shaker() throw();                 //!< cleanup
            virtual void    reset()              throw() = 0; //!< restart state
            virtual uint8_t fetch(const uint8_t) throw() = 0; //!< byte wise process

            //------------------------------------------------------------------
            //
            // non virtual interface
            //
            //------------------------------------------------------------------

            //! reset and substitute
            void apply(void *target, const void *source, const size_t length) throw();

            //! reset and in place substitute
            void apply(void *target, const size_t length) throw();

            //! reset and in place substitute
            void apply( memory::rw_buffer &buffer ) throw();

        protected:
            explicit shaker() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(shaker);
        };
    }


}

#endif

