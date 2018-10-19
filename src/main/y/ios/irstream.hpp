//! \file
#ifndef Y_IOS_IRSTREAM_INCLUDED
#define Y_IOS_IRSTREAM_INCLUDED 1


#include "y/ios/istream.hpp"
#include "y/fs/local-file.hpp"

namespace upsylon
{
    namespace ios
    {
        //! input raw stream
        class irstream : public istream, public local_file
        {
        public:
            //! open a file stream
            explicit irstream(const string &filename, const offset_t shift=0);

            //! open a file stream
            explicit irstream(const char   *filename, const offset_t shift=0);

            //! map from cstdin
            explicit irstream( const ios::cstdin_t & );

            //! desctructor
            virtual ~irstream() throw();

            virtual bool query(char &C); //!< get one char
            virtual void store(char  C); //!< go back one char, no check

        private:
            Y_DISABLE_COPY_AND_ASSIGN(irstream);
            int             cache;
            io_buffer       iobuf;
            uint8_t        *curr;
            const uint8_t  *last;
            //bool            _eof;
        };
    }
}

#endif

