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
            virtual ~ohstream() throw();                //!< destructor
            explicit ohstream( hashing::function &H );  //!< setup
            virtual void write( char C );               //!< H(C)
            virtual void flush();                       //!< do nothing

            hashing::function &hf;                      //!< host hashing function

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ohstream);
        };


    }
}

#endif

