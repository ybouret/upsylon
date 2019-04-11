//! \file
#ifndef Y_IOS_OHSTREAM_INCLUDED
#define Y_IOS_OHSTREAM_INCLUDED 1

#include "y/ios/serializable.hpp"
#include "y/ios/ostream.hpp"
#include "y/hashing/function.hpp"

namespace upsylon
{
    namespace ios
    {

        //! lightweight output hashing stream
        class ohstream : public ostream
        {
        public:
            virtual ~ohstream() throw();
            explicit ohstream( hashing::function &H );

            hashing::function &hf;

            virtual void write( char C );
            virtual void flush();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ohstream);
        };


    }
}

#endif

