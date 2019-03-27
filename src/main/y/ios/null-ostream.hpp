
//! \file
#ifndef Y_IOS_NULL_OSTREAM_INCLUDED
#define Y_IOS_NULL_OSTREAM_INCLUDED 1

#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace ios
    {
        //! interface for output streams
        class null_ostream : public ostream
        {
        public:
            explicit null_ostream() throw();
            virtual ~null_ostream() throw();
            virtual void write(char);
            virtual void flush();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(null_ostream);
        };
        
    }
}

#endif

