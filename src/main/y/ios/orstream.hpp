//! \file

#ifndef Y_IOS_ORSTREAM_INCLUDED
#define Y_IOS_ORSTREAM_INCLUDED 1

#include "y/ios/ostream.hpp"
#include "y/fs/local-file.hpp"

namespace upsylon
{
    namespace ios
    {

        //! output raw stream
        class orstream : public ostream, public local_file
        {
        public:
            explicit orstream( const string &filename, bool append=false);
            explicit orstream( const char   *filename, bool append=false);
            orstream( ios::cstdout_t & );
            orstream( ios::cstderr_t & );
            virtual ~orstream() throw();
            virtual void write(char C);
            virtual void flush();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(orstream);
        };

    }
}


#endif

