
//! \file
#ifndef Y_IOS_OMSTREAM_INCLUDED
#define Y_IOS_OMSTREAM_INCLUDED 1

#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace ios
    {
        //! output memory stream
        class omstream : public ostream
        {
        public:
            virtual ~omstream()                                throw(); //!< destructor
            explicit omstream( void *addr, const size_t size ) throw(); //!< setup
            explicit omstream( memory::rw_buffer &buff )       throw(); //!< setup

            virtual void write( char C ); //!< append to content
            virtual void flush();         //!< nothing to do

            const size_t       count;     //!< number of written bytes
            const size_t       space;     //!< initial data space
        private:
            Y_DISABLE_COPY_AND_ASSIGN(omstream);
            char              *entry;
        };
    }
}

#endif

