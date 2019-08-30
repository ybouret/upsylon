//! \file

#ifndef Y_IOS_OVSTREAM_INCLUDED
#define Y_IOS_OVSTREAM_INCLUDED 1

#include "y/sequence/vector.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace ios
    {
        //! base class for in-memory byte stream
        typedef vector<uint8_t,memory::global> ovstream_;

        //! output vector stream
        class ovstream : public ovstream_, public ios::ostream
        {
        public:
            explicit ovstream() throw(); //!< no memory
            explicit ovstream(const size_t n, const bool filled=false); //!< capacity>=n, filled or not
            virtual ~ovstream() throw(); //!< destructor

            ovstream &set_fast(const size_t n); //!< adjust with zero
            ovstream &set_zero(const size_t n); //!< set all to zero

            virtual void write( char C ); //!< push_back
            virtual void flush();         //!< ...
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ovstream);
        };


    }

}


#endif

