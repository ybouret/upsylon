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
            explicit orstream( const string &filename, bool append=false); //!< open a file
            explicit orstream( const char   *filename, bool append=false); //!< open a file
            orstream( ios::cstdout_t & ); //!< map to stdout
            orstream( ios::cstderr_t & ); //!< map to stderr
            virtual ~orstream() throw();  //!< destruct
            virtual void write(char C);   //!< ostream:write
            virtual void flush();         //!< ostream:flush

        private:
            Y_DISABLE_COPY_AND_ASSIGN(orstream);
            io_buffer      iobuf;
            uint8_t       *curr;
            const uint8_t *last;
        };

    }
}


#endif

