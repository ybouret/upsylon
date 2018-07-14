//! \file
#ifndef Y_MEMORY_BUFFER_INCLUDED
#define Y_MEMORY_BUFFER_INCLUDED 1

#include "y/object.hpp"

namespace upsylon
{
    namespace memory
    {
        //! read-only buffer
        class ro_buffer : public virtual object
        {
        public:
            virtual ~ro_buffer() throw(); //!< destructor

            //! read only memory of length()
            virtual const void *ro()     const throw() = 0;

            //! valid length() byte
            virtual size_t      length() const throw() = 0;

            //! endless byte reading
            inline uint8_t get_byte(const size_t i) const throw()
            {
                return (i>=length()) ? 0 : *(static_cast<const uint8_t*>(ro())+i);
            }
            
        protected:
            explicit ro_buffer() throw(); //!< constructor

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ro_buffer);

        };

        //! read-write buffer
        class rw_buffer : public ro_buffer
        {
        public:
            virtual ~rw_buffer() throw(); //!< destructor

            void *rw() throw(); //!< assuming writable area
            void  reverse() throw(); //!< using mreverse
        protected:
            explicit rw_buffer() throw(); //!< constructor

        private:
            Y_DISABLE_COPY_AND_ASSIGN(rw_buffer);
        };
    }
}

#endif

