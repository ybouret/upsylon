//! \file
#ifndef Y_IOS_OSTREAM_INCLUDED
#define Y_IOS_OSTREAM_INCLUDED 1

#include "y/strfwd.hpp"
#include "y/ios/stream.hpp"
#include "y/os/endian.hpp"
#include "y/os/static-check.hpp"
#include "y/os/printf-check.hpp"
#include "y/memory/buffer.hpp"
#include "y/ios/upacker.hpp"

namespace upsylon
{
    namespace ios
    {
    

        //! interface for output streams
        class ostream : public stream
        {
        public:
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________

            //! destructor
            virtual ~ostream() throw();

            //! write one char
            virtual void write( char C ) = 0;

            //! flush
            virtual void flush()         = 0;

            //! printf style, may be superseeded
            virtual ostream & operator()(const char *fmt,...) Y_PRINTF_CHECK(2,3);

            //__________________________________________________________________
            //
            // non-virtual interface : output utilities
            //__________________________________________________________________

            //! output large buffer
            void output(const void *buffer, const size_t buflen);

            //! flux for one char
            ostream & operator<<( const char );
            
            //! output text
            ostream & operator<<( const char *buffer );

            //! output buffer
            ostream & operator<<(const memory::ro_buffer &buffer);

            //! output binary address
            ostream & viz( const void *addr );
            
         
            //! repeat a char
            inline ostream & repeat( size_t sz, const char C)
            {
                while(sz-->0) write(C);
                return *this;
            }

            //! pad something for alignement
            inline ostream & align(const char  *buffer,
                                   const size_t buflen,
                                   const size_t aligned, const char C=' ')
            {
                assert(!(0==buffer&&buflen>0));
                for(size_t i=0;i<buflen;++i)       write(buffer[i]);
                for(size_t i=buflen;i<aligned;++i) write(C);
                return *this;
            }

            //! pad text
            ostream & align(const char *text,const size_t aligned,const char C=' ');

            //! pad text
            ostream & align(const string &s,const size_t aligned,const char C=' ');

            //__________________________________________________________________
            //
            // non-virtual interface : byte-wise output
            //__________________________________________________________________
          
            //! emit network byte order integral type
            template <typename T> inline
            size_t write_nbo(T x)
            {
                x = swap_be_as<T>(x);
                output(&x,sizeof(T));
                return sizeof(T);
            }
            
            //! emit compact unsigned
            template <typename T> inline
            size_t write_upack(T x)
            {
                //______________________________________________________________
                //
                // check value metrics
                //______________________________________________________________
                Y_STATIC_CHECK(sizeof(T)<=8,T_is_too_large);
                size_t n = 0;
                write(upacker::encode::init(x,n));
                const size_t written = 1+n;
                while(n-- > 0)
                     write( upacker::encode::next(x) );
                return written;
            }
            
            //! emit a binary block, return written bytes
            size_t write_block( const void *data, const size_t size );

            //! emit a binary block, return written bytes
            size_t write_block( const memory::ro_buffer  &buff );

            //! emit a binary text
            size_t write_block( const char *text );

        protected:
            //! constructor
            explicit ostream() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ostream);
        };
    }
}

#endif
