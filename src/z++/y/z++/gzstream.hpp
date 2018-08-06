#ifndef Y_GZSTREAM_INCLUDED
#define Y_GZSTREAM_INCLUDED 1

#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {
    
    namespace zlib {
        
        class gzstream {
        public:
            virtual ~gzstream() throw();
            
        protected:
            explicit gzstream( void *__GZ ) throw();
            void *GZ;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(gzstream);
        };
        
    }
    
    namespace ios
    {
        
        class igzstream : public zlib::gzstream, public istream
        {
            
        public:
            explicit igzstream( const string &file_name);
            explicit igzstream( const char   *file_name);
            virtual ~igzstream() throw();
            
            virtual bool query( char &C );
            virtual void store( char C  );
        private:
            Y_DISABLE_COPY_AND_ASSIGN(igzstream);
        };
        
        class ogzstream : public zlib::gzstream, public ostream
        {
            
        public:
            explicit ogzstream( const string &file_name, size_t level, bool append = false );
            explicit ogzstream( const char   *file_name, size_t level, bool append = false );
            virtual ~ogzstream() throw();
            
            virtual void write( char C  );
            virtual void flush();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ogzstream);
        };
        
        
    }
    
}



#endif
