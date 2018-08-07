//! \file
#ifndef Y_IOS_ICSTREAM_INCLUDED
#define Y_IOS_ICSTREAM_INCLUDED 1

#include "y/ios/istream.hpp"
#include "y/ios/cfile.hpp"

namespace upsylon
{
    namespace ios
    {
        //! input stream interface
        class icstream : public istream
        {
        public:
            //! destructor
            virtual ~icstream() throw();

            //! query a new char if any
            virtual bool query( char &C );

            //! unread a char
            virtual void store( const char C );

            //! constructor
            icstream(const string &filename);

            //! constructor
            icstream(const char *filename);

            //! map to STDIN
            icstream(const cstdin_t &);

            //! transitive file handle
            FILE * operator*() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(icstream);
            cfile handle;
        };
    }
}

#endif
