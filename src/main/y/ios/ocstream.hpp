
//! \file
#ifndef Y_IOS_OCSTREAM_INCLUDED
#define Y_IOS_OCSTREAM_INCLUDED 1

#include "y/ios/ostream.hpp"
#include "y/ios/cfile.hpp"

namespace upsylon
{
    namespace ios
    {
        //! interface for output streams
        class ocstream : public ostream
        {
        public:
            //! destructor
            virtual ~ocstream() throw();

            //! open a file
            ocstream(const string &filename, const bool append=false);

            //! open a file
            ocstream(const char   *filename, const bool append=false);

            //! use stderr
            ocstream( const ios::cstderr_t & );

            //! use stdout
            ocstream( const ios::cstdout_t & );

            //! write
            virtual void write(char c);

            //! flush
            virtual void flush();

            //! direct formatting
            virtual ostream & operator()(const char *fmt,...) Y_PRINTF_CHECK(2,3);

            //! overwrite
            static void overwrite(const string &filename);

            //! overwrite
            static void overwrite(const char *filename);

            //! echo append
            static void echo(const string &filename, const char *fmt,...) Y_PRINTF_CHECK(2,3);

            //! echo append
            static void echo(const char   *filename, const char *fmt,...) Y_PRINTF_CHECK(2,3);

            //! transitive file handle
            FILE * operator*() throw();

        private:
            cfile handle;

        };

    }
}

#endif

