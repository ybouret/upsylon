#ifndef Y_BZ2STREAM_INCLUDED
#define Y_BZ2STREAM_INCLUDED 1


#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/bz2/bzlib.h"

namespace upsylon
{
    
    namespace bzlib
    {
        class bz2stream
        {
        public:
            
            int      bzerror;
            virtual ~bz2stream() throw();
            const char *get_error() const throw();
            
        protected:
            explicit bz2stream() throw(); //!< bzfile to NULL
            BZFILE  *bzfile;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(bz2stream);
            
        };
     
        class ibz2stream : public bz2stream, public ios::istream
        {
        public:
            explicit ibz2stream( const string &filename );
            explicit ibz2stream( const ios::cstdin_t &);
            virtual ~ibz2stream() throw();
            
            virtual bool query( char &C );
			virtual void store( const char  C );
            
        private:
            ios::icstream fp;
            Y_DISABLE_COPY_AND_ASSIGN(ibz2stream);
            int           cache;
            bool          alive;
        public:
            int *last_close;
        };
        
        
    }
    
}

#endif
