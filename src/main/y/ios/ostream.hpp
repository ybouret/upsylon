//! \file
#ifndef Y_IOS_OSTREAM_INCLUDED
#define Y_IOS_OSTREAM_INCLUDED 1

#include "y/strfwd.hpp"
#include "y/ios/stream.hpp"
#include "y/os/endian.hpp"
#include "y/os/static-check.hpp"
#include "y/os/printf-check.hpp"
#include "y/memory/buffer.hpp"
#include "y/ios/gist.hpp"

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
            // Virtual Interface
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
            // Non-Virtual Interface
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

            //! emit integral types in network byte order, optional ASSIGN count
            template <typename T> inline
            ostream & emit_net(T x, size_t *count=NULL)
            {
                x = swap_be_as<T>(x);
                output(&x,sizeof(T));
                gist::assign(count,sizeof(T));
                return *this;
            }

            //! emit compact unsigned, optional ASSIGN count
            template <typename T> inline
            ostream & emit_upack(T x, size_t *count=NULL)
            {
                //______________________________________________________________
                //
                // check value metrics
                //______________________________________________________________
                Y_STATIC_CHECK(sizeof(T)<=8,T_is_too_large);
                const size_t  num_bits     = bits_for(x); assert(num_bits<=64);
                const uint8_t last4shifted = uint8_t(x&T(0x0f)) << 4;
                if(num_bits<=4)
                {
                    //__________________________________________________________
                    //
                    // 0 extra bytes!
                    //__________________________________________________________
                    write(last4shifted);
                    gist::assign(count,1);
                }
                else
                {
                    //__________________________________________________________
                    //
                    // some extra bits/byte(s)
                    //__________________________________________________________
                    const size_t extra_bits  = num_bits - 4;
                    size_t       extra_bytes = Y_ROUND8(extra_bits)>>3; assert(extra_bytes<=8);
                    write( char(last4shifted | extra_bytes) );
                    gist::assign(count,1);
                    x >>= 4;
                    while(extra_bytes-->0)
                    {
                        const uint8_t B = uint8_t(x&T(0xff));
                        write(B);
                        gist::shr8<T>(x, int2type< (sizeof(T)>1) >() );
                        gist::add_to(count,1);
                    }
                    assert(0==x);
                }

                return *this;
            }

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

            //! emit a binary block, return written bytes
            size_t emit_block( const void *data, const size_t size );

            //! emit a binary block, return written bytes
            size_t emit_block( const memory::ro_buffer  &buff );

            //! emit a binary text
            size_t emit_block( const char *text );

        protected:
            //! constructor
            explicit ostream() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ostream);
        };
    }
}

//! wrapper to increment data count
#define Y_OSTREAM_ADD_TO(COUNT,METHOD,DATA) do { size_t count=0; (void) METHOD(DATA,&count); (COUNT) += count; } while(false)

#endif
