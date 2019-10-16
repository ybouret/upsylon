//! \file
#ifndef Y_MEMORY_BUFFER_INCLUDED
#define Y_MEMORY_BUFFER_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    namespace memory
    {
        //! read-only buffer
        class ro_buffer
        {
        public:
            //------------------------------------------------------------------
            // virtual interface
            //------------------------------------------------------------------
            virtual            ~ro_buffer()             throw();     //!< destructor
            virtual const void *ro()              const throw() = 0; //!< read only memory of length()
            virtual size_t      length()          const throw() = 0; //!< valid length() byte
            //------------------------------------------------------------------
            // non virtual interface
            //------------------------------------------------------------------
            uint8_t             byte_at(size_t i) const throw();     //!< virtual never ending byte access
        protected:
            explicit ro_buffer() throw();                            //!< constructor

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ro_buffer);
        };

        //! read-write buffer
        class rw_buffer : public ro_buffer
        {
        public:
            virtual ~rw_buffer() throw(); //!< destructor
            void *   rw()        throw(); //!< assuming writable area
            void     reverse()   throw(); //!< using mreverse

        protected:
            explicit rw_buffer() throw(); //!< constructor

        private:
            Y_DISABLE_COPY_AND_ASSIGN(rw_buffer);
        };
    }
}

#endif

