//! \file
#ifndef Y_IOS_OSTREAM_INCLUDED
#define Y_IOS_OSTREAM_INCLUDED 1

#include "y/ios/stream.hpp"
#include "y/string.hpp"
#include "y/os/endian.hpp"

namespace upsylon
{
    namespace ios
    {
        //! interface for output streams
        class ostream : public stream
        {
        public:
            //! destructor
            virtual ~ostream() throw();

            //! write one char
            virtual void write( char C ) = 0;

            //! flush
            virtual void flush()         = 0;

            //! printf style, may be superseeded
            virtual void operator()(const char *fmt,...) Y_PRINTF_CHECK(2,3);


            //! output large buffer
            inline void output(const char *buffer, const size_t buflen)
            {
                assert(!(buffer==0&&buflen>0));
                for(size_t i=0;i<buflen;++i) write(buffer[i]);
            }
            
            //! flux for one char
            inline ostream & operator<<( const char C )
            {
                write(C);
                return *this;
            }
            
            //! output text
            inline ostream & operator<<( const char *buffer )
            {
                output(buffer,length_of(buffer));
                return *this;
            }

            //! output buffer
            inline ostream & operator<<(const memory::ro_buffer &buffer)
            {
                output( static_cast<const char *>(buffer.ro()), buffer.length() );
                return *this;
            }

            //! output binary address
            ostream & viz( const void *addr );

            //! emit integral types
            template <typename T>
            ostream & emit(T x)
            {
                x = swap_be_as<T>(x);
                output((const char*)&x,sizeof(T));
                return *this;
            }

        protected:
            //! constructor
            explicit ostream() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ostream);
        };
    }
}

#endif
